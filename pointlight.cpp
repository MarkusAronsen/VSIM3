#include "pointlight.h"

PointLight::PointLight(gsl::Vec3 position, gsl::Vec3 ambientColor, gsl::Vec3 diffuseColor, gsl::Vec3 specularColor) //, float ambientStrength, float specularStrength)
    : mAmbientColor{ambientColor}, mDiffuseColor{diffuseColor}, mSpecularColor{specularColor},
      //mAmbientStrenght{ambientStrength}, mSpecularStrength{specularStrength},
      mPosition{position}
{

}

gsl::Vec3 PointLight::getPosition() const
{
    return mPosition;
}

void PointLight::setPosition(const gsl::Vec3 &position)
{
    mPosition = position;
}
