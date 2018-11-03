#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

class SystemBase
{
public:
    SystemBase();
    virtual ~SystemBase();

    virtual void init();
    virtual void draw();
    virtual void update();
    unsigned int getSystemID() const;

private:
    static unsigned int ID;
    unsigned int mSystemID;

};

#endif // SYSTEMBASE_H
