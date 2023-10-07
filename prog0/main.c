#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Point struct with X and Y fields
typedef struct
{
  double x;
  double y;
} Point;

// void printDoubleArray(double *arr, int length)
// {
//   for (int i = 0; i < length; ++i)
//   {
//     printf("%f ", arr[i]);
//   }
//   printf("\n");
// }

// curve
// inputs: Point A, Point B - The starting and ending points of the line
// depth - The current depth of recursion
// maxDepth - The maximum depth of recursion
// cr: the cairo surface to draw on once the maxDepth has been reached
void curve(Point A, Point B, int depth, int maxDepth, cairo_t *cr)
{
  // if depth is equal to the maximum recursive depth, then
  // draw the points
  if (depth == maxDepth)
  {
    // draw all the lines using Cairo
    cairo_set_source_rgb(cr, 1, 0, 0); // red
    cairo_move_to(cr, A.x, A.y);       // top left corner
    cairo_line_to(cr, B.x, B.y);       // middle of the image
    cairo_set_line_width(cr, 1);
    cairo_stroke(cr);

    // printf("A is (%.2f, %.2f), B is (%.2f, %.2f)\n", A.x, A.y, B.x, B.y);
  }
  else // otherwise, call itself recursively till max depth is reached
  {
    double ePrimeX = B.x - A.x;
    double ePrimeY = B.y - A.y;

    double vX = ((ePrimeX + ePrimeY) / 2) + A.x;
    double vY = ((-ePrimeX + ePrimeY) / 2) + A.y;

    Point V;
    V.x = vX;
    V.y = vY;

    // Recursive call
    // two lines, from start point to new point,
    curve(A, V, depth + 1, maxDepth, cr);
    // from end point to new point
    curve(V, B, depth + 1, maxDepth, cr);
  }
}

int main(int argc, char *argv[])
{

  // TODO: error checking on params?
  if (argc < 9)
  {
    printf("Please include all arguments.\n");
    printf("Arguments - filename, width, height, depth, Ax, Ay, Bx, By (starting point A and ending point B).\n");
    return -1;
  }

  // Start Params ====
  char *filename = argv[1];
  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  // Maximum depth
  double depth = atoi(argv[4]); // needs to be positive number

  // starting point A
  double Ax = atof(argv[5]);
  double Ay = atof(argv[6]);
  Point A;
  A.x = Ax;
  A.y = Ay;

  // ending point B
  double Bx = atof(argv[7]);
  double By = atof(argv[8]);
  Point B;
  B.x = Bx;
  B.y = By;
  // End Params ====

  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(surface);

  // Create the background
  cairo_set_source_rgb(cr, 1, 1, 1); // white
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);

  // Call the recursive curve function to draw the levy c curve
  curve(A, B, 0, depth, cr);

  // finally, clean up and save the file
  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, strcat(filename, ".png"));
  cairo_surface_destroy(surface);
  return 0;
}
