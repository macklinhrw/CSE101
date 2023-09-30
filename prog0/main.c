#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} Point;

void printDoubleArray(double *arr) {
  int size = sizeof(arr) / sizeof(arr[0]);
  printf("size: %d", size);
  for (int i = 0; i < size; ++i) {
    printf("%f ", arr[i]);
  }
  printf("\n");
}

double *curve() {
  static double array[] = {0, 0};
  return array;
}

int main(int argc, char *argv[]) {
  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 512, 512);
  cairo_t *cr = cairo_create(surface);

  if (argc < 8) {
    printf("Please include all arguments.\n");
    // arguments - width, height, ...

    return -1;
  }

  // Start Params ====
  char *filename = argv[1];
  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  // starting point A
  double Ax = atof(argv[4]);
  double Ay = atof(argv[5]);
  Point A;
  A.x = Ax;
  A.y = Ay;

  // starting point B
  double Bx = atof(argv[6]);
  double By = atof(argv[7]);
  Point B;
  B.x = Bx;
  B.y = By;
  // End Params ====

  printf("filename: %s\n", filename);
  printf("width: %d\n", width);
  printf("height: %d\n", height);

  double *array = curve();
  printDoubleArray(array);

  return 0;

  // process command line input per specification

  // default is black background.  set background to white.
  // set pen to black

  // top left corner -- green X on black background
  // default background is black

  cairo_set_source_rgb(cr, 0, 1, 0); // green
  cairo_move_to(cr, 0, 0);           // top left corner
  cairo_line_to(cr, 256, 256);       // middle of the image
  cairo_move_to(cr, 256, 0);
  cairo_line_to(cr, 0, 256);
  cairo_set_line_width(cr, 10.0);
  cairo_stroke(cr);

  // bottom left corner -- white box on blue background using
  // coordinate specification

  cairo_set_source_rgb(cr, 0, 0, 1); // blue
  cairo_rectangle(cr, 0, 255, 255, 255);
  cairo_fill(cr);

  cairo_move_to(cr, 0, 255);
  cairo_line_to(cr, 255, 255);
  cairo_line_to(cr, 255, 511);
  cairo_line_to(cr, 0, 511);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 1, 1, 1); // white
  cairo_set_line_width(cr, 8.0);
  cairo_stroke(cr);

  // bottom right corner -- red box on white background using
  // relative line drawing

  cairo_set_source_rgb(cr, 1, 1, 1); // white
  cairo_rectangle(cr, 256, 256, 511, 511);
  cairo_fill(cr);

  cairo_move_to(cr, 256, 256);
  cairo_rel_line_to(cr, 256, 0);
  cairo_rel_line_to(cr, 0, 256);
  cairo_rel_line_to(cr, -256, 0);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 1, 0, 0); // red
  cairo_set_line_width(cr, 5.5);
  cairo_stroke(cr);

  // send output to png file
  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, "test.png");
  cairo_surface_destroy(surface);
  return 0;
}
