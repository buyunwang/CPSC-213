int src[2] = {1,0};

void copy() {
  int dst[2];
  int i = 0;
  while (src[i] != 0) {
    dst[i] = src[i];
    i++;
  }
  dst[i]=0;
}

int main () {
  copy ();
}