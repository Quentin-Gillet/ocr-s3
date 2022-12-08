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
    gtk_image_set_from_file(info->image,filename); // filename est un const char*
    
    
    // chargement de l'image
    //info->image = xxx;
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
    infos.image = image;
    printf("%p\n",image);
    printf("%p\n",file_chooser);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(create_image), &infos);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
