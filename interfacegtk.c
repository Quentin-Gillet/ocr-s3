#include <gtk/gtk.h>

typedef struct appinfo {
    GtkImage* image;
} AppInfo;

void create_image(GtkFileChooserButton* file_chooser_button, gpointer user_data)
{
    AppInfo* info = user_data;
    GFile* file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER  (file_chooser_button) );
    const char * filename = g_file_get_path(file); 
    printf("filename : %s",filename);
    const GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file(filename,NULL);
    GdkPixbuf * ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf,1000,850,GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(info->image,ResizedPixbuf); // filename est un const char*
    
}

// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);
    AppInfo infos;

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface1.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
	
    // Gets the widgets.
    // = prend les élements de glade
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window")); 
    GtkFileChooserButton* file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
    GtkImage * image = GTK_IMAGE(gtk_builder_get_object(builder,"ImageToPrint"));
    GtkButton * Nextbutton = GTK_BUTTON(gtk_builder_get_object(builder,"NextButton"));
    GtkLevelBar * ProgressBar = GTK_LEVEL_BAR(gtk_builder_get_object(builder,"ProgressBar"));
    GtkLabel * ProcessLabel = GTK_LABEL(gtk_builder_get_object(builder,"ProcessLabel"));

    infos.image = image;

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(create_image), &infos);
    
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
