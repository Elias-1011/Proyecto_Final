#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
public:
    Entidad(float x, float y);
    virtual ~Entidad();

    virtual void actualizar(float dt) = 0;

    float getX() const { return x; }
    float getY() const { return y; }
    void  setX(float nx) { x = nx; }
    void  setY(float ny) { y = ny; }

    bool estaActivo() const { return activo; }
    void desactivar()       { activo = false; }

protected:
    float x, y;
    bool  activo;
};

#endif
