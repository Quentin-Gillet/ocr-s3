#include <gtk/gtk.h>

typedef struct appinfo {
    GtkImage *image;
    int CurrEvent;
    GtkButton *Nextbutton;
    GtkLevelBar *ProgressBar;
    GtkButton *Skipbutton;
    GtkButton *Resetbutton;
    GtkLabel *ProcessLabel;
} AppInfo;

void set_image(const char * filename, GtkImage * image)
{
    const GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf, 1000, 850, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, ResizedPixbuf); // filename est un const char*
}

void create_image(GtkFileChooserButton *file_chooser_button, gpointer user_data)
{
    AppInfo *info = user_data;
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER  (file_chooser_button));
    const char *filename = g_file_get_path(file);
    printf("filename : %s\n", filename);
    set_image(filename,info->image);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
    
    /*
    const GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf, 1000, 850, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(info->image, ResizedPixbuf); // filename est un const char*
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
     */
}

void reset(GtkButton * Resetbutton, gpointer user_data)
{
	AppInfo *info = user_data;
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);

}

void next_event(GtkButton *Nextbutton, gpointer user_data) {
    AppInfo *info = user_data;
    info->CurrEvent = info->CurrEvent + 1;
    printf("currevent : %i\n", info->CurrEvent);
    gtk_button_set_label(Nextbutton, "Next Step");
    double BarValue = gtk_level_bar_get_value(info->ProgressBar);
    gtk_level_bar_set_value(info->ProgressBar,BarValue+10);
    GtkLabel *ProcessLabel = info->ProcessLabel;
    switch (info->CurrEvent) {
        case 0:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            gtk_label_set_label(ProcessLabel, "Applying Greyscale filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 1:

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Contrast filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 2:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Mean filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 3:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Blur filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 4:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Inverted filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 5:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Sobel filter...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;
        case 6:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Hough Line...");
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

    }

}

void skip(GtkButton * Skipbutton, gpointer user_data)
{
	 AppInfo *info = user_data;
	 gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
	 gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), FALSE);
}

// Main function.
int main(int argc, char *argv[]) {
    // Initializes GTK.
    gtk_init(NULL, NULL);
    AppInfo infos;

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "interface1.glade", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkCssProvider * cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);

    GdkScreen * screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);


    // Gets the widgets.
    // = prend les élements de glade
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkFileChooserButton *file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
    GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "ImageToPrint"));
    GtkButton *Nextbutton = GTK_BUTTON(gtk_builder_get_object(builder, "NextButton"));
    GtkLevelBar *ProgressBar = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "ProgressBar"));
    GtkLabel *ProcessLabel = GTK_LABEL(gtk_builder_get_object(builder, "ProcessLabel"));
    GtkButton * Skipbutton = GTK_BUTTON(gtk_builder_get_object(builder, "SkipButton"));
    GtkButton * Resetbutton = GTK_BUTTON(gtk_builder_get_object(builder, "ResetButton"));

    infos.image = image;
    infos.CurrEvent = -1;
    infos.Nextbutton = Nextbutton;
    infos.ProgressBar = ProgressBar;
    infos.ProcessLabel = ProcessLabel;
    infos.Skipbutton = Skipbutton;
    infos.Resetbutton = Resetbutton;

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(create_image), &infos);
    g_signal_connect(Nextbutton, "clicked", G_CALLBACK(next_event), &infos);
    g_signal_connect(Skipbutton, "clicked", G_CALLBACK(skip), &infos);
    g_signal_connect(Resetbutton, "clicked", G_CALLBACK(reset), &infos);
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
