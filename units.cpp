#include "units.h"

using namespace units;

#define MM2M  0.001
#define FT2M  0.3048
#define IN2M  0.0254
#define FT2MM 304.8
#define IN2MM 25.4
#define IN2FT 0.0833333
#define M2IN  39.37007874

quint64 ConvertToInt(qreal val)
{
    return quint64((val + 0.00005) * 10000.0);
}

qreal ConvertToReal(quint64 val)
{
    return (qreal(val) / 10000.0);
}




unit::unit()
{}

qreal unit::value()
{
    return _val;
}



meters::meters()
{
    _val = 0.0;
    _type = m;
}

meters::meters(qreal val)
{
    _val = val;
    _type = m;
}

meters::meters(quint32 val)
{
    _val = qreal(val);
    _type = m;
}

meters::meters(meters& val)
{
    _val = val._val;
    _type = m;
}
meters::meters(millimeters val)
{
    _val = (val._val * MM2M);
    _type = m;
}
meters::meters(feet val)
{
    _val = val._val * FT2M;
    _type = m;
}
meters::meters(inches val)
{
    _val = val._val * IN2M;
    _type = m;
}

meters& meters::operator=  (qint32 val)
{
    _val = (qreal)val;

    return *this;
}

meters& meters::operator=  (qreal val)
{
    _val = val;

    return *this;
}

meters& meters::operator=  (meters val)
{
    _val = val._val;

    return *this;
}

meters& meters::operator+ (meters val)
{
    meters* temp = new meters(_val + val._val);

    return *temp;
}

meters& meters::operator+= (meters val)
{
    _val += val._val;

    return *this;
}

meters& meters::operator- (meters val)
{
    meters* temp = new meters(_val - val._val);

    return *temp;
}

meters& meters::operator-= (meters val)
{
    _val -= val._val;

    return *this;
}
#if 0
meters& meters::operator+  (millimeters val)
{
    meters* temp = new meters(val);
    temp->_val = _val + temp->_val;

    return *temp;
}

meters& meters::operator+= (millimeters val)
{
    _val += val._val;

    return *this;
}

meters& meters::operator-  (millimeters val)
{
    meters* temp = new meters(val);
    temp->_val = _val - temp->_val;

    return *temp;
}

meters& meters::operator-= (millimeters val)
{
    _val -= val._val;

    return *this;
}
#endif





millimeters::millimeters(qreal val)
{
    _val = val;
    _type = mm;
}

millimeters::millimeters(quint32 val)
{
    _val = (qreal) val;
    _type = mm;
}
millimeters::millimeters(meters val)
{
    _val = val._val * MM2M;
    _type = mm;
}
millimeters::millimeters(millimeters& val)
{
    _val = val._val;
    _type = mm;
}
millimeters::millimeters(feet val)
{
    _val = val._val * FT2MM;
    _type = mm;
}
millimeters::millimeters(inches val)
{
    _val = val._val * IN2MM;
    _type = mm;
}

millimeters& millimeters::operator+ (millimeters val)
{
    millimeters* temp = new millimeters(_val + val._val);

    return *temp;
}

millimeters& millimeters::operator+= (millimeters val)
{
    _val += val._val;

    return *this;
}

millimeters& millimeters::operator- (millimeters val)
{
    millimeters* temp = new millimeters(_val + val._val);

    return *temp;
}
millimeters& millimeters::operator-= (millimeters val)
{
    _val -= val._val;

    return *this;
}




feet::feet(qreal val)
{
    _val = (qreal)val;
    _type = ft;
}

feet::feet(feet& val)
{
    _val = val._val;
    _type = ft;
}

feet::feet(meters val)
{
    _val = val._val / FT2M;
    _type = ft;
}

feet::feet(millimeters val)
{
    _val = val._val / FT2MM;
    _type = ft;
}

feet::feet(inches val)
{
    _val = val._val * IN2FT;
    _type = ft;
}

feet& feet::operator+ (feet val)
{
    feet* temp = new feet(_val + val._val);

    return *temp;
}

feet& feet::operator+= (feet val)
{
    _val += val._val;

    return *this;
}

feet& feet::operator- (feet val)
{
    feet* temp = new feet(_val - val._val);

    return *temp;
}

feet& feet::operator-= (feet val)
{
    _val -= val._val;

    return *this;
}




inches::inches(qreal val)
{
    _val = (qreal)val;
    _type = in;
}

inches::inches(quint32 val)
{
    _val = (qreal)val;
    _type = in;
}

inches::inches(inches& val)
{
    _val = val._val;
    _type = in;
}

inches::inches(meters val)
{
    _val = val._val * M2IN;
    _type = in;
}

inches::inches(millimeters val)
{
    _val = val._val * (M2IN / MM2M);
    _type = in;
}

inches::inches(feet val)
{
    _val = val._val / IN2FT;
    _type = in;
}

inches& inches::operator+ (inches val)
{
    inches* temp = new inches(_val + val._val);

    return *temp;
}

inches& inches::operator+= (inches val)
{
    _val += val._val;

    return *this;
}

inches& inches::operator- (inches val)
{
    inches* temp = new inches(_val - val._val);

    return *temp;
}

inches& inches::operator-= (inches val)
{
    _val -= val._val;

    return *this;

}
