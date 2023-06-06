#include "affine_transformations/s21_affine_transformations.h"
#include "affine_transformations/s21_matrix.h"

int main() {
  s21::AffineTransformations af;
  af.MakeMovement();
  af.print();
  return 0;
}