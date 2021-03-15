typedef enum { false, true } bool;
bool is_sorted(int *a, int n) {
  int i;
  for (i = 0; i < n - 1; i++)
    if (a[i] > a[i + 1])
      return false;
  return true;
}
void main(){}
