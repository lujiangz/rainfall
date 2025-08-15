#include <cstring> 
#include <cstdlib>

class N {

private:
    char annotation[100];
    
    int value;

public:
    N(int val) : value(val) {
    }

    void setAnnotation(char* text) {
        size_t text_length = strlen(text);
        memcpy(this->annotation, text, text_length);
    }


    virtual int operator+(const N& other) {
        return this->value + other.value;
    }

    virtual int operator-(const N& other) {
        return this->value - other.value;
    }
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        exit(1);
    }

    N* n1 = new N(5);

    N* n2 = new N(6);

    n1->setAnnotation(argv[1]);

    *n2 + *n1; // (*(code *)**(undefined4 **)this_00)(this_00,this);

    return 0;
}