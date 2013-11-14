//vector.h
#ifndef __vector_h_
#define __vector_h_

class vector{
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;

        unsigned int first_index;
        unsigned int second_index;

    vector(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) : x(_x), y(_y), z(_z), w(_w) {}
    vector(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z), w(0.0) {}
    vector(GLfloat _x, GLfloat _y) : x(_x), y(_y), z(0.0), w(0.0) {}
    vector() : x(0.0), y(0.0), z(0.0), w(0.0) {}

    GLfloat& operator[](int index){
        if(index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else return w;
    }

    //matrix assignment
    vector& operator=(vector &rhs){
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    //copy matrix rhs to this matrix
    void copy(vector rhs){
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
    }

    void matrixVectorMultiply(GLfloat rmat[16], vector& new_vec){
        int count = 3;
        GLfloat newMatrix[16];
        for(int i = 0; i < 16; i+=4){
            newMatrix[count] = (rmat[i] * x) + (rmat[i+1] * y) + (rmat[i+2] * z) + (rmat[i+3] * w);
            count+=4;
        }
        new_vec.x = newMatrix[3]; new_vec.y = newMatrix[7]; new_vec.z = newMatrix[11]; new_vec.w = newMatrix[15];
    }

    void rotateVector(GLfloat angle, vector& new_vec){
        GLfloat rmat[] =   {cos(angle * (3.14/180)), 0.0, sin(angle * (3.14/180)), 0.0,
                            0.0,        1.0,        0.0, 0.0,
                            -sin(angle * (3.14/180)),0.0, cos(angle * (3.14/180)), 0.0,
                            0.0,        0.0,        0.0, 1.0};
        matrixVectorMultiply(rmat, new_vec);
    }

    void crossProduct(vector& a, vector& b){
        this->x = a.y*b.z - a.z*b.y;
        this->y = a.z*b.x - a.x*b.z;
        this->z = a.x*b.y - a.y*b.x;
    }

    void setIndices(int first, int second){
        first_index = first; second_index = second;
    }
};
#endif