$NetBSD: patch-gio_gunixmounts.c,v 1.1 2015/06/10 23:00:05 prlw1 Exp $

Revert commit 548c165a Make GUnixMountMonitor per-context
https://bugzilla.gnome.org/show_bug.cgi?id=750708

--- gio/gunixmounts.c.orig	2015-03-13 20:48:21.000000000 +0000
+++ gio/gunixmounts.c
@@ -68,7 +68,6 @@
 #include "gfilemonitor.h"
 #include "glibintl.h"
 #include "gthemedicon.h"
-#include "gcontextspecificgroup.h"
 
 
 #ifdef HAVE_MNTENT_H
@@ -1275,50 +1274,124 @@ static guint signals[LAST_SIGNAL];
 struct _GUnixMountMonitor {
   GObject parent;
 
-  GMainContext *context;
+  GFileMonitor *fstab_monitor;
+  GFileMonitor *mtab_monitor;
+
+  GList *mount_poller_mounts;
+
+  GSource *proc_mounts_watch_source;
 };
 
 struct _GUnixMountMonitorClass {
   GObjectClass parent_class;
 };
 
+static GUnixMountMonitor *the_mount_monitor = NULL;
 
 G_DEFINE_TYPE (GUnixMountMonitor, g_unix_mount_monitor, G_TYPE_OBJECT);
 
-static GContextSpecificGroup  mount_monitor_group;
-static GFileMonitor          *fstab_monitor;
-static GFileMonitor          *mtab_monitor;
-static GSource               *proc_mounts_watch_source;
-static GList                 *mount_poller_mounts;
+static void
+g_unix_mount_monitor_finalize (GObject *object)
+{
+  GUnixMountMonitor *monitor;
+  
+  monitor = G_UNIX_MOUNT_MONITOR (object);
+
+  if (monitor->fstab_monitor)
+    {
+      g_file_monitor_cancel (monitor->fstab_monitor);
+      g_object_unref (monitor->fstab_monitor);
+    }
+  
+  if (monitor->proc_mounts_watch_source != NULL)
+    g_source_destroy (monitor->proc_mounts_watch_source);
+
+  if (monitor->mtab_monitor)
+    {
+      g_file_monitor_cancel (monitor->mtab_monitor);
+      g_object_unref (monitor->mtab_monitor);
+    }
+
+  g_list_free_full (monitor->mount_poller_mounts, (GDestroyNotify)g_unix_mount_free);
+
+  the_mount_monitor = NULL;
+
+  G_OBJECT_CLASS (g_unix_mount_monitor_parent_class)->finalize (object);
+}
+
+
+static void
+g_unix_mount_monitor_class_init (GUnixMountMonitorClass *klass)
+{
+  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
+
+  gobject_class->finalize = g_unix_mount_monitor_finalize;
+ 
+  /**
+   * GUnixMountMonitor::mounts-changed:
+   * @monitor: the object on which the signal is emitted
+   * 
+   * Emitted when the unix mounts have changed.
+   */ 
+  signals[MOUNTS_CHANGED] =
+    g_signal_new ("mounts-changed",
+		  G_TYPE_FROM_CLASS (klass),
+		  G_SIGNAL_RUN_LAST,
+		  0,
+		  NULL, NULL,
+		  g_cclosure_marshal_VOID__VOID,
+		  G_TYPE_NONE, 0);
+
+  /**
+   * GUnixMountMonitor::mountpoints-changed:
+   * @monitor: the object on which the signal is emitted
+   * 
+   * Emitted when the unix mount points have changed.
+   */
+  signals[MOUNTPOINTS_CHANGED] =
+    g_signal_new ("mountpoints-changed",
+		  G_TYPE_FROM_CLASS (klass),
+		  G_SIGNAL_RUN_LAST,
+		  0,
+		  NULL, NULL,
+		  g_cclosure_marshal_VOID__VOID,
+		  G_TYPE_NONE, 0);
+}
 
 static void
 fstab_file_changed (GFileMonitor      *monitor,
-                    GFile             *file,
-                    GFile             *other_file,
-                    GFileMonitorEvent  event_type,
-                    gpointer           user_data)
+		    GFile             *file,
+		    GFile             *other_file,
+		    GFileMonitorEvent  event_type,
+		    gpointer           user_data)
 {
+  GUnixMountMonitor *mount_monitor;
+
   if (event_type != G_FILE_MONITOR_EVENT_CHANGED &&
       event_type != G_FILE_MONITOR_EVENT_CREATED &&
       event_type != G_FILE_MONITOR_EVENT_DELETED)
     return;
 
-  g_context_specific_group_emit (&mount_monitor_group, signals[MOUNTPOINTS_CHANGED]);
+  mount_monitor = user_data;
+  g_signal_emit (mount_monitor, signals[MOUNTPOINTS_CHANGED], 0);
 }
 
 static void
 mtab_file_changed (GFileMonitor      *monitor,
-                   GFile             *file,
-                   GFile             *other_file,
-                   GFileMonitorEvent  event_type,
-                   gpointer           user_data)
+		   GFile             *file,
+		   GFile             *other_file,
+		   GFileMonitorEvent  event_type,
+		   gpointer           user_data)
 {
+  GUnixMountMonitor *mount_monitor;
+
   if (event_type != G_FILE_MONITOR_EVENT_CHANGED &&
       event_type != G_FILE_MONITOR_EVENT_CREATED &&
       event_type != G_FILE_MONITOR_EVENT_DELETED)
     return;
-
-  g_context_specific_group_emit (&mount_monitor_group, signals[MOUNTS_CHANGED]);
+  
+  mount_monitor = user_data;
+  g_signal_emit (mount_monitor, signals[MOUNTS_CHANGED], 0);
 }
 
 static gboolean
@@ -1326,21 +1399,23 @@ proc_mounts_changed (GIOChannel   *chann
                      GIOCondition  cond,
                      gpointer      user_data)
 {
+  GUnixMountMonitor *mount_monitor = G_UNIX_MOUNT_MONITOR (user_data);
   if (cond & G_IO_ERR)
-    g_context_specific_group_emit (&mount_monitor_group, signals[MOUNTS_CHANGED]);
-
+    g_signal_emit (mount_monitor, signals[MOUNTS_CHANGED], 0);
   return TRUE;
 }
 
 static gboolean
 mount_change_poller (gpointer user_data)
 {
+  GUnixMountMonitor *mount_monitor;
   GList *current_mounts, *new_it, *old_it;
   gboolean has_changed = FALSE;
 
+  mount_monitor = user_data;
   current_mounts = _g_get_unix_mounts ();
 
-  for ( new_it = current_mounts, old_it = mount_poller_mounts;
+  for ( new_it = current_mounts, old_it = mount_monitor->mount_poller_mounts;
         new_it != NULL && old_it != NULL;
         new_it = g_list_next (new_it), old_it = g_list_next (old_it) )
     {
@@ -1353,55 +1428,34 @@ mount_change_poller (gpointer user_data)
   if (!(new_it == NULL && old_it == NULL))
     has_changed = TRUE;
 
-  g_list_free_full (mount_poller_mounts, (GDestroyNotify) g_unix_mount_free);
+  g_list_free_full (mount_monitor->mount_poller_mounts,
+                    (GDestroyNotify)g_unix_mount_free);
 
-  mount_poller_mounts = current_mounts;
+  mount_monitor->mount_poller_mounts = current_mounts;
 
   if (has_changed)
     {
-      mount_poller_time = (guint64) g_get_monotonic_time ();
-      g_context_specific_group_emit (&mount_monitor_group, signals[MOUNTPOINTS_CHANGED]);
+      mount_poller_time = (guint64)g_get_monotonic_time ();
+      g_signal_emit (mount_monitor, signals[MOUNTS_CHANGED], 0);
     }
 
   return TRUE;
 }
 
-
-static void
-mount_monitor_stop (void)
-{
-  if (fstab_monitor)
-    {
-      g_file_monitor_cancel (fstab_monitor);
-      g_object_unref (fstab_monitor);
-    }
-
-  if (proc_mounts_watch_source != NULL)
-    g_source_destroy (proc_mounts_watch_source);
-
-  if (mtab_monitor)
-    {
-      g_file_monitor_cancel (mtab_monitor);
-      g_object_unref (mtab_monitor);
-    }
-
-  g_list_free_full (mount_poller_mounts, (GDestroyNotify) g_unix_mount_free);
-}
-
 static void
-mount_monitor_start (void)
+g_unix_mount_monitor_init (GUnixMountMonitor *monitor)
 {
   GFile *file;
-
+    
   if (get_fstab_file () != NULL)
     {
       file = g_file_new_for_path (get_fstab_file ());
-      fstab_monitor = g_file_monitor_file (file, 0, NULL, NULL);
+      monitor->fstab_monitor = g_file_monitor_file (file, 0, NULL, NULL);
       g_object_unref (file);
-
-      g_signal_connect (fstab_monitor, "changed", (GCallback)fstab_file_changed, NULL);
+      
+      g_signal_connect (monitor->fstab_monitor, "changed", (GCallback)fstab_file_changed, monitor);
     }
-
+  
   if (get_mtab_monitor_file () != NULL)
     {
       const gchar *mtab_path;
@@ -1423,93 +1477,39 @@ mount_monitor_start (void)
             }
           else
             {
-              proc_mounts_watch_source = g_io_create_watch (proc_mounts_channel, G_IO_ERR);
-              g_source_set_callback (proc_mounts_watch_source,
+              monitor->proc_mounts_watch_source = g_io_create_watch (proc_mounts_channel, G_IO_ERR);
+              g_source_set_callback (monitor->proc_mounts_watch_source,
                                      (GSourceFunc) proc_mounts_changed,
-                                     NULL, NULL);
-              g_source_attach (proc_mounts_watch_source,
+                                     monitor,
+                                     NULL);
+              g_source_attach (monitor->proc_mounts_watch_source,
                                g_main_context_get_thread_default ());
-              g_source_unref (proc_mounts_watch_source);
+              g_source_unref (monitor->proc_mounts_watch_source);
               g_io_channel_unref (proc_mounts_channel);
             }
         }
       else
         {
           file = g_file_new_for_path (mtab_path);
-          mtab_monitor = g_file_monitor_file (file, 0, NULL, NULL);
+          monitor->mtab_monitor = g_file_monitor_file (file, 0, NULL, NULL);
           g_object_unref (file);
-          g_signal_connect (mtab_monitor, "changed", (GCallback)mtab_file_changed, NULL);
+          g_signal_connect (monitor->mtab_monitor, "changed", (GCallback)mtab_file_changed, monitor);
         }
     }
   else
     {
-      proc_mounts_watch_source = g_timeout_source_new_seconds (3);
-      mount_poller_mounts = _g_get_unix_mounts ();
+      monitor->proc_mounts_watch_source = g_timeout_source_new_seconds (3);
+      monitor->mount_poller_mounts = _g_get_unix_mounts ();
       mount_poller_time = (guint64)g_get_monotonic_time ();
-      g_source_set_callback (proc_mounts_watch_source,
-                             mount_change_poller,
-                             NULL, NULL);
-      g_source_attach (proc_mounts_watch_source,
+      g_source_set_callback (monitor->proc_mounts_watch_source,
+                             (GSourceFunc)mount_change_poller,
+                             monitor, NULL);
+      g_source_attach (monitor->proc_mounts_watch_source,
                        g_main_context_get_thread_default ());
-      g_source_unref (proc_mounts_watch_source);
+      g_source_unref (monitor->proc_mounts_watch_source);
     }
 }
 
-static void
-g_unix_mount_monitor_finalize (GObject *object)
-{
-  GUnixMountMonitor *monitor;
-
-  monitor = G_UNIX_MOUNT_MONITOR (object);
-
-  g_context_specific_group_remove (&mount_monitor_group, monitor->context, monitor, mount_monitor_stop);
-
-  G_OBJECT_CLASS (g_unix_mount_monitor_parent_class)->finalize (object);
-}
-
-static void
-g_unix_mount_monitor_class_init (GUnixMountMonitorClass *klass)
-{
-  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
-
-  gobject_class->finalize = g_unix_mount_monitor_finalize;
- 
-  /**
-   * GUnixMountMonitor::mounts-changed:
-   * @monitor: the object on which the signal is emitted
-   * 
-   * Emitted when the unix mounts have changed.
-   */ 
-  signals[MOUNTS_CHANGED] =
-    g_signal_new ("mounts-changed",
-		  G_TYPE_FROM_CLASS (klass),
-		  G_SIGNAL_RUN_LAST,
-		  0,
-		  NULL, NULL,
-		  g_cclosure_marshal_VOID__VOID,
-		  G_TYPE_NONE, 0);
-
-  /**
-   * GUnixMountMonitor::mountpoints-changed:
-   * @monitor: the object on which the signal is emitted
-   * 
-   * Emitted when the unix mount points have changed.
-   */
-  signals[MOUNTPOINTS_CHANGED] =
-    g_signal_new ("mountpoints-changed",
-		  G_TYPE_FROM_CLASS (klass),
-		  G_SIGNAL_RUN_LAST,
-		  0,
-		  NULL, NULL,
-		  g_cclosure_marshal_VOID__VOID,
-		  G_TYPE_NONE, 0);
-}
-
-static void
-g_unix_mount_monitor_init (GUnixMountMonitor *monitor)
-{
-}
-
 /**
  * g_unix_mount_monitor_set_rate_limit:
  * @mount_monitor: a #GUnixMountMonitor
@@ -1537,16 +1537,12 @@ g_unix_mount_monitor_set_rate_limit (GUn
 /**
  * g_unix_mount_monitor_get:
  *
- * Gets the #GUnixMountMonitor for the current thread-default main
- * context.
+ * Gets the #GUnixMountMonitor.
  *
  * The mount monitor can be used to monitor for changes to the list of
  * mounted filesystems as well as the list of mount points (ie: fstab
  * entries).
  *
- * You must only call g_object_unref() on the return value from under
- * the same main context as you called this function.
- *
  * Returns: (transfer full): the #GUnixMountMonitor.
  *
  * Since: 2.44
@@ -1554,10 +1550,13 @@ g_unix_mount_monitor_set_rate_limit (GUn
 GUnixMountMonitor *
 g_unix_mount_monitor_get (void)
 {
-  return g_context_specific_group_get (&mount_monitor_group,
-                                       G_TYPE_UNIX_MOUNT_MONITOR,
-                                       G_STRUCT_OFFSET(GUnixMountMonitor, context),
-                                       mount_monitor_start);
+  if (the_mount_monitor == NULL)
+    {
+      the_mount_monitor = g_object_new (G_TYPE_UNIX_MOUNT_MONITOR, NULL);
+      return the_mount_monitor;
+    }
+  
+  return g_object_ref (the_mount_monitor);
 }
 
 /**
