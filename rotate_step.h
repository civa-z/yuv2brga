

    class Rotate0 {
    public:
        Rotate0(int rows, int cols, int channels) :
                start_shift(0),
                row_step(cols * channels),
                col_step(channels){ }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate0Mirror {
    public:
        Rotate0Mirror(int rows, int cols, int channels) :
                start_shift((cols - 1) * channels),
                row_step(cols * channels),
                col_step(-channels){ }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate90 {
    public:
        Rotate90(int rows, int cols, int channels) :
                start_shift((rows - 1) * channels),
                row_step(-channels),
                col_step(rows * channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate90Mirror {
    public:
        Rotate90Mirror(int rows, int cols, int channels) :
                start_shift(0),
                row_step(channels),
                col_step(rows * channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate180 {
    public:
        Rotate180(int rows, int cols, int channels) :
                start_shift((rows * cols - 1) * channels),
                row_step(-cols * channels),
                col_step(-channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate180Mirror {
    public:
        Rotate180Mirror(int rows, int cols, int channels) :
                start_shift(((rows - 1) * cols) * channels),
                row_step(-cols * channels),
                col_step(channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate270 {
    public:
        Rotate270(int rows, int cols, int channels) :
                start_shift(rows * (cols - 1) * channels),
                row_step(channels),
                col_step(-rows * channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

    class Rotate270Mirror {
    public:
        Rotate270Mirror(int rows, int cols, int channels) :
                start_shift((rows * cols - 1) * channels),
                row_step(-channels),
                col_step(-rows * channels) { }

        const int start_shift;
        const int row_step;
        const int col_step;
    };

