//
// Created by Alexis COGNET on 10/12/2022.
//

#ifndef OCR_INTERFACEGTK_H
#define OCR_INTERFACEGTK_H

#include <gtk/gtk.h>
#include "image_processing/image_rotation.h"
#include "image_processing/line_detection.h"
#include "image_processing/image_split.h"
#include "image_processing/image_processing.h"

typedef struct AppInfo {
    GtkImage *image;
    GtkFileChooserButton *file_chooser;
    int CurrEvent;
    GtkButton *Nextbutton;
    GtkLevelBar *ProgressBar;
    GtkButton *Skipbutton;
    GtkButton *Resetbutton;
    GtkLabel *ProcessLabel;
    Image rawImage;
    Image CpImage;
    Image Image_Cells;
    Line * lines;
    int linesLength;
} AppInfo;


void skip(GtkButton *Skipbutton, gpointer user_data);
void next_event(GtkButton *Nextbutton, gpointer user_data);
void create_image(GtkFileChooserButton *file_chooser_button, gpointer user_data);
void set_image(const char *filename, GtkImage *image);
void reset(GtkButton *Resetbutton, gpointer user_data);
int start_gui();

#endif //OCR_INTERFACEGTK_H
