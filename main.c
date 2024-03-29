// Libraries
#include <gtk-3.0/gtk/gtk.h>  // GTK Library
#include <stdio.h>
#include <stdlib.h>
#include <xdo.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Objects
GtkWidget *uiwindow;   // Window
GtkLabel *uiselected;  // Label
GtkEntry *text[5];
GtkEntry *uitime;
GtkButton *uistartbutton;

// Variables
struct timespec *req;
struct timespec *rem;
struct timespec *current_time;
struct timespec *saved_time;
int intervall = 10;

int toggle = 0;
int start_toggle = 0;
char *state[2] = {"Start", "Pause"};
xdo_t *x;
Window *window;
FILE *fptr;

const gchar *gui =
"<interface>"
"  <requires lib=\"gtk+\" version=\"3.20\"/>"
"  <object class=\"GtkWindow\" id=\"uiwindow\">"
"    <property name=\"can_focus\">False</property>"
"    <property name=\"title\" translatable=\"yes\">Linuxtyper</property>"
"    <signal name=\"destroy\" handler=\"exit_app\" swapped=\"no\"/>"
"    <child type=\"titlebar\">"
"      <placeholder/>"
"    </child>"
"    <child>"
"      <object class=\"GtkBox\">"
"        <property name=\"visible\">True</property>"
"        <property name=\"can_focus\">False</property>"
"        <property name=\"orientation\">vertical</property>"
"        <property name=\"spacing\">10</property>"
"        <child>"
"          <object class=\"GtkBox\">"
"            <property name=\"visible\">True</property>"
"            <property name=\"can_focus\">False</property>"
"            <property name=\"margin_left\">10</property>"
"            <property name=\"margin_right\">10</property>"
"            <property name=\"margin_top\">10</property>"
"            <property name=\"margin_bottom\">10</property>"
"            <property name=\"orientation\">vertical</property>"
"            <child>"
"              <object class=\"GtkBox\">"
"                <property name=\"visible\">True</property>"
"                <property name=\"can_focus\">False</property>"
"                <property name=\"orientation\">vertical</property>"
"                <property name=\"spacing\">5</property>"
"                <property name=\"homogeneous\">True</property>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitext1\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">0</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitext2\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">1</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitext3\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">2</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitext4\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">3</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitext5\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">False</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">4</property>"
"                  </packing>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name=\"expand\">True</property>"
"                <property name=\"fill\">True</property>"
"                <property name=\"position\">0</property>"
"              </packing>"
"            </child>"
"          </object>"
"          <packing>"
"            <property name=\"expand\">True</property>"
"            <property name=\"fill\">True</property>"
"            <property name=\"position\">1</property>"
"          </packing>"
"        </child>"
"        <child>"
"          <object class=\"GtkBox\">"
"            <property name=\"visible\">True</property>"
"            <property name=\"can_focus\">False</property>"
"            <property name=\"margin_left\">10</property>"
"            <property name=\"margin_right\">10</property>"
"            <property name=\"margin_top\">10</property>"
"            <property name=\"margin_bottom\">10</property>"
"            <property name=\"spacing\">50</property>"
"            <property name=\"homogeneous\">True</property>"
"            <child>"
"              <object class=\"GtkGrid\">"
"                <property name=\"visible\">True</property>"
"                <property name=\"can_focus\">False</property>"
"                <property name=\"row_spacing\">10</property>"
"                <property name=\"column_spacing\">10</property>"
"                <property name=\"row_homogeneous\">True</property>"
"                <child>"
"                  <object class=\"GtkButton\" id=\"uiselectwindow\">"
"                    <property name=\"label\" translatable=\"yes\">Select a window</property>"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                    <property name=\"receives_default\">True</property>"
"                    <property name=\"halign\">start</property>"
"                    <property name=\"valign\">center</property>"
"                    <signal name=\"clicked\" handler=\"select_clicked\" swapped=\"no\"/>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">0</property>"
"                    <property name=\"top_attach\">2</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkSwitch\" id=\"uitoggleminimize\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                    <property name=\"halign\">start</property>"
"                    <property name=\"valign\">center</property>"
"                    <signal name=\"state-set\" handler=\"minimize_toggled\" swapped=\"no\"/>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">1</property>"
"                    <property name=\"top_attach\">0</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkEntry\" id=\"uitime\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                    <property name=\"valign\">center</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">1</property>"
"                    <property name=\"top_attach\">1</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkLabel\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">False</property>"
"                    <property name=\"halign\">start</property>"
"                    <property name=\"valign\">center</property>"
"                    <property name=\"label\" translatable=\"yes\">Minimize window:</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">0</property>"
"                    <property name=\"top_attach\">0</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkLabel\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">False</property>"
"                    <property name=\"halign\">start</property>"
"                    <property name=\"valign\">center</property>"
"                    <property name=\"label\" translatable=\"yes\">Time in seconds:</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">0</property>"
"                    <property name=\"top_attach\">1</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class=\"GtkLabel\" id=\"uiselected\">"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">False</property>"
"                    <property name=\"label\" translatable=\"yes\">No window selected</property>"
"                  </object>"
"                  <packing>"
"                    <property name=\"left_attach\">1</property>"
"                    <property name=\"top_attach\">2</property>"
"                  </packing>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name=\"expand\">True</property>"
"                <property name=\"fill\">True</property>"
"                <property name=\"position\">0</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class=\"GtkBox\">"
"                <property name=\"visible\">True</property>"
"                <property name=\"can_focus\">False</property>"
"                <property name=\"orientation\">vertical</property>"
"                <child>"
"                  <object class=\"GtkButton\" id=\"uistartbutton\">"
"                    <property name=\"label\" translatable=\"yes\">Start</property>"
"                    <property name=\"visible\">True</property>"
"                    <property name=\"can_focus\">True</property>"
"                    <property name=\"receives_default\">True</property>"
"                    <signal name=\"clicked\" handler=\"start_clicked\" swapped=\"no\"/>"
"                  </object>"
"                  <packing>"
"                    <property name=\"expand\">True</property>"
"                    <property name=\"fill\">True</property>"
"                    <property name=\"position\">0</property>"
"                  </packing>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name=\"expand\">True</property>"
"                <property name=\"fill\">True</property>"
"                <property name=\"position\">1</property>"
"              </packing>"
"            </child>"
"          </object>"
"          <packing>"
"            <property name=\"expand\">False</property>"
"            <property name=\"fill\">True</property>"
"            <property name=\"position\">2</property>"
"          </packing>"
"        </child>"
"      </object>"
"    </child>"
"  </object>"
"</interface>";

void idle() {

}

void func() {
        clock_gettime(CLOCK_REALTIME, current_time);
    while ((current_time->tv_sec > (saved_time->tv_sec + intervall)) && (start_toggle != 0)) {
        if (xdo_get_pid_window(x, *window) == 0) {
            xdo_get_focused_window(x, window);
        }
        xdo_activate_window(x, *window);
        xdo_send_keysequence_window(x, *window, "Return", 2);

        nanosleep(req, rem);

        char *str = gtk_entry_get_text(text[rand() % 5]);
        if (str[0] == '\0') {
            str = "o/";
        }
        xdo_enter_text_window(x, *window, str, 0);
        xdo_send_keysequence_window(x, *window, "Return", 2);

        if (toggle != 0) {
            nanosleep(req, rem);
            xdo_minimize_window(x, *window);
            xdo_send_keysequence_window(x, *window, "Return", 2);
        }

        printf("sleeping for %d seconds...\n", intervall);
        clock_gettime(CLOCK_REALTIME, saved_time);
    }
}

// Main function
int main(int argc, char *argv[]) {
    GError *err = NULL;
    GtkBuilder *builder;     // GTK Builder variable
    gtk_init(&argc, &argv);  // Start GTK

    builder = gtk_builder_new();  // Create GTK UI Builder
    if (0 == gtk_builder_add_from_string(builder, gui, -1, &err)) {
        /* Print out the error. You can use GLib's message logging */
        fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
        /* Your error handling code goes here */
    }

    // Assign the Variables
    uiwindow = GTK_WIDGET(gtk_builder_get_object(builder, "uiwindow"));  // Load our window named MyWindow
    if (NULL == uiwindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"uiwindow\" \n");
        /* Your error handling code goes here */
    }

    text[0] = GTK_ENTRY(gtk_builder_get_object(builder, "uitext1"));
    text[1] = GTK_ENTRY(gtk_builder_get_object(builder, "uitext2"));
    text[2] = GTK_ENTRY(gtk_builder_get_object(builder, "uitext3"));
    text[3] = GTK_ENTRY(gtk_builder_get_object(builder, "uitext4"));
    text[4] = GTK_ENTRY(gtk_builder_get_object(builder, "uitext5"));

    uiselected = GTK_LABEL(gtk_builder_get_object(builder, "uiselected"));  // Load our label named MyLabel
    uitime = GTK_ENTRY(gtk_builder_get_object(builder, "uitime"));
    uistartbutton = GTK_BUTTON(gtk_builder_get_object(builder, "uistartbutton"));

    // Essential for a GTK based program
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    // Setup
    char *line = NULL;
    size_t len = 0;
    int i = 0;

    fptr = fopen("./.local/share/linuxtyper.txt", "a+");
    while ((getline(&line, &len, fptr)) != -1) {
        line[strlen(line) - 1] = '\0';
        gtk_entry_set_text(text[i], line);
        i++;
    }
    free(line);

    x = xdo_new(":0.0");
    window = (Window *)malloc(sizeof(Window));

    req = (struct timespec *)malloc(sizeof(struct timespec));
    req->tv_sec = 200 / 1000;
    req->tv_nsec = (200 % 1000) * 1000000;
    rem = (struct timespec *)malloc(sizeof(struct timespec));

    current_time = (struct timespec *)malloc(sizeof(struct timespec));
    saved_time = (struct timespec *)malloc(sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, saved_time);

    // Run our program
    gtk_widget_show_all(uiwindow);  // Show our window
    gtk_main();
    gdk_threads_add_idle((GSourceFunc)idle, NULL);

    return 0;  // Necessary for a c main function
}

// Function to exit our app
void exit_app() {
    printf("Exit app \n");  // Not neccesary
    fptr = fopen("./.local/share/linuxtyper.txt", "w");
    for (int i = 0; i < 5; i++) {
        fputs(gtk_entry_get_text(text[i]), fptr);
        fputs("\n", fptr);
    }

    gtk_main_quit();  // Command to quit a GTK program
}

// Function when our button is pressed
void start_clicked() {
    start_toggle = !start_toggle;
    gtk_button_set_label(uistartbutton, state[start_toggle]);
    printf("tstart %d\n", start_toggle);
    gtk_widget_set_sensitive(GTK_WIDGET(uitime), TRUE);

    if (start_toggle == 1) {
        gtk_widget_set_sensitive(GTK_WIDGET(uitime), FALSE);
        intervall = atoi(gtk_entry_get_text(uitime)) + 10 * !(atoi(gtk_entry_get_text(uitime)));
        clock_gettime(CLOCK_REALTIME, saved_time);
        // gdk_threads_add_idle((GSourceFunc)func, NULL);
    }
}

void minimize_toggled() {
    toggle = !toggle;
    printf("toggle the min %d\n", toggle);
}

void select_clicked() {
    char str[10];
    printf("pleek choose \n");
    for (int i = 3; i > 0; i--) {
        sleep(1);
        xdo_get_focused_window(x, window);
        printf("%d \n", i);
    }
    printf("done choose \n");
    int pid = xdo_get_pid_window(x, *window);
    sprintf(str, "%d", pid);
    gtk_label_set_text(uiselected, str);
}