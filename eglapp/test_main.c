int main_cube(int argc, char **argv);
int main_gles20(int argc, char **argv);
int main_opengl(int argc, char **argv);

int main(int argc, char **argv)
{
    return main_opengl(argc, argv);
}