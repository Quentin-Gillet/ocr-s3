#include "interface/interfacegtk.h"

void set_image(const char *filename, GtkImage *image)
{
    const GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *ResizedPixbuf = gdk_pixbuf_scale_simple(pixbuf, 1000, 850, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, ResizedPixbuf); // filename is a const char*
    if (pixbuf != NULL)
        g_object_unref(pixbuf);
}

void create_image(GtkFileChooserButton *file_chooser_button, gpointer user_data)
{
    AppInfo *info = user_data;

    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser_button));
    const char *filename = g_file_get_path(file);
    set_image(filename, info->image);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), TRUE);
    info->rawImage = getImageFromPng(filename);
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

    GtkLabel *ProcessLabel = info->ProcessLabel;

    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(info->file_chooser));
    if (file == NULL)
    {
        gtk_label_set_label(ProcessLabel, "Please input an image in the file chooser");
        return;
    }

    gtk_widget_set_sensitive(GTK_WIDGET(info->Skipbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Skipbutton), FALSE);

    info->CurrEvent = info->CurrEvent + 1;
    gtk_button_set_label(Nextbutton, "Next Step");

    double BarValue = gtk_level_bar_get_value(info->ProgressBar);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 11.11111111);

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

            free(info->lines);
            freeImage(&info->cpImage);
            freeImage(&info->imageCells);
            freeImage(&info->rawImage);
            break;

        case 8:
            gtk_label_set_label(ProcessLabel, "Generating solution...");
            Recognition_Solve();
            set_image("images/sudokuPresentation.result.bmp", info->image);
            break;

        default:
            gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
            gtk_label_set_label(ProcessLabel, "End of solver (PUSH RESET TO RESTART)");
            break;

    }

}

void skip(GtkButton *Skipbutton, gpointer user_data)
{
    AppInfo *info = user_data;

    GtkLabel *ProcessLabel = info->ProcessLabel;
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(info->file_chooser));
    if (file == NULL)
    {
        gtk_label_set_label(ProcessLabel, "Please input an image in the file chooser");
        return;
    }

    if(info->CurrEvent != -1)
    {
        return;
    }

    gtk_widget_set_sensitive(GTK_WIDGET(info->Nextbutton), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), FALSE);

    double BarValue = gtk_level_bar_get_value(info->ProgressBar);

    gtk_label_set_label(ProcessLabel, "Applying Greyscale filter...");
    imageGrayscale(&info->rawImage);
    saveImageToBmp(&info->rawImage, "greyscale", "");
    set_image("images/greyscale.bmp", info->image);

    gtk_label_set_label(ProcessLabel, "Applying Contrast filter...");
    imageContrastFilter(&info->rawImage);
    saveImageToBmp(&info->rawImage, "contrast", "");
    set_image("images/contrast.bmp", info->image);

    gtk_label_set_label(ProcessLabel, "Applying Mean filter...");
    imageBinarization(&info->rawImage);
    saveImageToBmp(&info->rawImage, "mean", "");
    Image cpImage = copyImage(&info->rawImage);
    set_image("images/mean.bmp", info->image);

    gtk_label_set_label(ProcessLabel, "Applying Inverted filter...");
    imageInvert(&info->rawImage);
    saveImageToBmp(&info->rawImage, "inverted", "");
    set_image("images/inverted.bmp", info->image);


    gtk_label_set_label(ProcessLabel, "Applying Sobel filter...");
    imageSobelFilter(&info->rawImage);
    saveImageToBmp(&info->rawImage, "sobel", "");
    set_image("images/sobel.bmp", info->image);

    gtk_label_set_label(ProcessLabel, "Applying Blur filter...");
    imageMedianBlur(&info->rawImage);
    saveImageToBmp(&info->rawImage, "blur", "");

    gtk_level_bar_set_value(info->ProgressBar, BarValue + 50);

    gtk_label_set_label(ProcessLabel, "Applying Hough Line...");
    int linesLength = 0;
    Line *lines = getImageLines(&info->rawImage, 450, &linesLength);

    drawLineOnImage(&info->rawImage, lines, linesLength);
    saveImageToBmp(&info->rawImage, "hough", "");
    set_image("images/hough.bmp", info->image);;

    gtk_label_set_label(ProcessLabel, "Searching Biggest Square...");
    //test detection carré
    Line *newlines2 = get_Bigger_Squares(lines, linesLength, info->cpImage.width, info->cpImage.height);
    //Line* newlines2 = print_squares(lines, linesLength);
    drawLineOnImage(&cpImage, newlines2, 4);
    saveImageToBmp(&cpImage, "biggest-rectangle", "");
    set_image("images/biggest-rectangle.bmp", info->image);

    gtk_label_set_label(ProcessLabel, "Generating solution...");
    Recognition_Solve();
    set_image("images/sudokuPresentation.result.bmp", info->image);
    gtk_level_bar_set_value(info->ProgressBar, BarValue + 100);

    gtk_widget_set_sensitive(GTK_WIDGET(info->Resetbutton), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(Skipbutton), FALSE);

    //Free all the memory
    freeImage(&info->rawImage);
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


    // Gets the widgets.
    // = take all the widgets from Glade
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkFileChooserButton *file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
    GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "ImageToPrint"));
    GtkButton *Nextbutton = GTK_BUTTON(gtk_builder_get_object(builder, "NextButton"));
    GtkLevelBar *ProgressBar = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "ProgressBar"));
    GtkLabel *ProcessLabel = GTK_LABEL(gtk_builder_get_object(builder, "ProcessLabel"));
    GtkButton *Skipbutton = GTK_BUTTON(gtk_builder_get_object(builder, "SkipButton"));
    GtkButton *Resetbutton = GTK_BUTTON(gtk_builder_get_object(builder, "ResetButton"));

    //Initializing the appInfos
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
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &infos);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(create_image), &infos);
    g_signal_connect(Nextbutton, "clicked", G_CALLBACK(next_event), &infos);
    g_signal_connect(Skipbutton, "clicked", G_CALLBACK(skip), &infos);
    g_signal_connect(Resetbutton, "clicked", G_CALLBACK(reset), &infos);
    // Runs the main loop.
    gtk_main();


    // Exits.
    return 0;
}
