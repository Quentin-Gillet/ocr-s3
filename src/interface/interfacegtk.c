#include "interface/interfacegtk.h"

void set_image(const char *filename, GtkImage *image)
{
    const GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf, 1000, 850, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, ResizedPixbuf); // filename is a const char*
}

void create_image(GtkFileChooserButton *file_chooser_button, gpointer user_data)
{
    AppInfo *info = user_data;

    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser_button));
    const char *filename = g_file_get_path(file);
    printf("filename : %s\n", filename);
    set_image(filename, info->image);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
    info->rawImage = getImageFromPng(filename);

    /*
    const GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf, 1000, 850, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(info->image, ResizedPixbuf); // filename is a const char*
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
     */
}

void reset(GtkButton *Resetbutton, gpointer user_data)
{
    AppInfo *info = user_data;

    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Skipbutton), FALSE);
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(info->file_chooser));
    const char *filename = g_file_get_path(file);
    set_image(filename, info->image);
    info->CurrEvent = -1;
    gtk_level_bar_set_value(info->ProgressBar, 0);
    GtkLabel *ProcessLabel = info->ProcessLabel;
    gtk_label_set_label(ProcessLabel, "");

    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Skipbutton), TRUE);
    info->rawImage = getImageFromPng(filename);

}

void next_event(GtkButton *Nextbutton, gpointer user_data)
{
    AppInfo *info = user_data;

    gtk_widget_set_sensitive(GTK_WIDGET(info->Skipbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), TRUE);

    info->CurrEvent = info->CurrEvent + 1;
    printf("currevent : %i\n", info->CurrEvent);
    gtk_button_set_label(Nextbutton, "Next Step");

    double BarValue = gtk_level_bar_get_value(info->ProgressBar);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);
    GtkLabel *ProcessLabel = info->ProcessLabel;

    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(info->file_chooser));
    const char *filename = g_file_get_basename(file);

    switch (info->CurrEvent)
    {

        case 0:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);

            gtk_label_set_label(ProcessLabel, "Applying Greyscale filter...");

            imageGrayscale(&info->rawImage);
            saveImageToBmp(&info->rawImage, "greyscale", "");
            set_image("images/greyscale.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 1:

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Contrast filter...");

            imageContrastFilter(&info->rawImage);
            saveImageToBmp(&info->rawImage, "contrast", "");
            set_image("images/contrast.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 2:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Mean filter...");

            imageBinarization(&info->rawImage);
            saveImageToBmp(&info->rawImage, "mean", "");
            info->imageCells = copyImage(&info->rawImage);
            info->cpImage = copyImage(&info->rawImage);
            set_image("images/mean.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 3:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Inverted filter...");

            imageInvert(&info->rawImage);
            saveImageToBmp(&info->rawImage, "inverted", "");
            set_image("images/inverted.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 4:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Sobel filter...");

            imageSobelFilter(&info->rawImage);
            saveImageToBmp(&info->rawImage, "sobel", "");
            set_image("images/sobel.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 5:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Blur filter...");

            imageMedianBlur(&info->rawImage);
            saveImageToBmp(&info->rawImage, "blur", "");
            set_image("images/blur.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 6:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "Applying Hough Line...");

            info->lines = getImageLines(&info->rawImage, 450, &info->linesLength);

            drawLineOnImage(&info->rawImage, info->lines, info->linesLength);
            saveImageToBmp(&info->rawImage, "hough", "");
            set_image("images/hough.bmp", info->image);

            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
            break;

        case 7:
            gtk_label_set_label(ProcessLabel, "Searching Biggest Square...");

            //test detection carré
            Line *newlines2 = get_Bigger_Squares(info->lines, info->linesLength, info->cpImage.width,
                                                 info->cpImage.height);
            //Line* newlines2 = print_squares(lines, linesLength);
            drawLineOnImage(&info->cpImage, newlines2, 4);
            saveImageToBmp(&info->cpImage, "biggest-rectangle", "");
            set_image("images/biggest-rectangle.bmp", info->image);
            break;

        case 8:

            break;

        default:
            //freeImage(image_cells);
            //free(lines);
            break;

    }

}

void quitApp(gpointer user_data)
{
    AppInfo *info = user_data;

    freeImage(&info->rawImage);
    freeImage(&info->cpImage);
    freeImage(&info->imageCells);
    if(info->lines != NULL)
        free(info->lines);
    gtk_main_quit();
}

void skip(GtkButton *Skipbutton, gpointer user_data)
{
    AppInfo *info = user_data;

    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), FALSE);

    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(info->file_chooser));
    const char *filename = g_file_get_basename(file);
    Image image = getImageFromPng(filename);

    double BarValue = gtk_level_bar_get_value(info->ProgressBar);
    GtkLabel *ProcessLabel = info->ProcessLabel;

    gtk_label_set_label(ProcessLabel, "Applying Greyscale filter...");
    imageGrayscale(&image);
    saveImageToBmp(&image, "greyscale", "");
    set_image("images/greyscale.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Contrast filter...");
    imageContrastFilter(&image);
    saveImageToBmp(&image, "contrast", "");
    set_image("images/contrast.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Mean filter...");
    imageBinarization(&image);
    saveImageToBmp(&image, "mean", "");
    Image cpImage = copyImage(&image);
    set_image("images/mean.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Inverted filter...");
    imageInvert(&image);
    saveImageToBmp(&image, "inverted", "");
    set_image("images/inverted.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Sobel filter...");
    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel", "");
    set_image("images/sobel.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Blur filter...");
    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur", "");
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 14.28571429);

    gtk_label_set_label(ProcessLabel, "Applying Hough Line...");
    int linesLength = 0;
    Line *lines = getImageLines(&image, 450, &linesLength);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough", "");
    set_image("images/hough.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 100);

    gtk_label_set_label(ProcessLabel, "Searching Biggest Square...");
    //test detection carré
    Line *newlines2 = get_Bigger_Squares(lines, linesLength, info->cpImage.width, info->cpImage.height);
    //Line* newlines2 = print_squares(lines, linesLength);
    drawLineOnImage(&cpImage, newlines2, 4);
    saveImageToBmp(&cpImage, "biggest-rectangle", "");
    set_image("images/biggest-rectangle.bmp", info->image);

    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(Skipbutton), FALSE);

    //Free all the memory
    freeImage(&image);
    freeImage(&cpImage);
    free(lines);
    free(newlines2);
}

// Main function.
int start_gui()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);
    AppInfo infos;

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "src/interface/interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/interface/style.css", NULL);

    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);


    // Gets the widgets.
    // = prend les élements de glade
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkFileChooserButton *file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
    GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "ImageToPrint"));
    GtkButton *Nextbutton = GTK_BUTTON(gtk_builder_get_object(builder, "NextButton"));
    GtkLevelBar *ProgressBar = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "ProgressBar"));
    GtkLabel *ProcessLabel = GTK_LABEL(gtk_builder_get_object(builder, "ProcessLabel"));
    GtkButton *Skipbutton = GTK_BUTTON(gtk_builder_get_object(builder, "SkipButton"));
    GtkButton *Resetbutton = GTK_BUTTON(gtk_builder_get_object(builder, "ResetButton"));

    infos.image = image;
    infos.CurrEvent = -1;
    infos.Nextbutton = Nextbutton;
    infos.ProgressBar = ProgressBar;
    infos.ProcessLabel = ProcessLabel;
    infos.Skipbutton = Skipbutton;
    infos.Resetbutton = Resetbutton;
    infos.file_chooser = file_chooser;
    infos.linesLength = 0;

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(quitApp), &infos);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(create_image), &infos);
    g_signal_connect(Nextbutton, "clicked", G_CALLBACK(next_event), &infos);
    g_signal_connect(Skipbutton, "clicked", G_CALLBACK(skip), &infos);
    g_signal_connect(Resetbutton, "clicked", G_CALLBACK(reset), &infos);
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
