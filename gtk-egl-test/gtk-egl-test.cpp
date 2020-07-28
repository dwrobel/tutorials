//
// Copyright (C) 2020  Damian Wrobel <dwrobel@ertelnet.rybnik.pl>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

// g++ -o gtk-egl-test gtk-egl-test.cpp $(pkg-config --cflags --libs gtk+-3.0 glesv2)

#include <GLES2/gl2.h>
#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  printf("main() window=%p\n", window);
  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_widget_add_events(window, 0);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *glarea = gtk_gl_area_new();
  gtk_container_add(GTK_CONTAINER(window), glarea);
  gtk_widget_add_events(glarea, 0);

  g_signal_connect(
      glarea, "realize", G_CALLBACK(+[](GtkGLArea *glarea) {
        printf("realize() glarea=%p\n", glarea);

        GError *error = gtk_gl_area_get_error(glarea);
        if (error) {
          printf("realize(1) error(%d,%d): %s\n", error->domain, error->code, error->message);
          return;
        }

        gtk_gl_area_make_current(glarea);

        error = gtk_gl_area_get_error(glarea);
        if (error) {
          printf("realize(2) error(%d.%d): %s\n", error->domain, error->code, error->message);
          return;
        }

        GdkGLContext *glcontext = gtk_gl_area_get_context(glarea);
        printf("realize() glcontext=%p\n", glcontext);
        GdkWindow *glwindow = gdk_gl_context_get_window(glcontext);
        printf("realize() glwindow=%p\n", glwindow);
        GdkFrameClock *frame_clock = gdk_window_get_frame_clock(glwindow);
        printf("realize() frame_clock=%p\n", frame_clock);

        g_signal_connect_swapped(frame_clock, "update",
                                 G_CALLBACK(gtk_gl_area_queue_render), glarea);

        gdk_frame_clock_begin_updating(frame_clock);
      }),
      NULL);

  g_signal_connect(glarea, "render", G_CALLBACK(+[](GtkGLArea *glarea) {
                     static int color = 0;
                     color = (color + 1) % 256;
                     const float c = color / 255.0;
                     glClearColor(0.0, c, 0.0, 1.0);
                     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                   }),
                   NULL);

  gtk_widget_show_all(window);

  gtk_main();

  exit(0);
}
