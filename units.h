#pragma once
#include <qglobal.h>
#include <QDebug>

namespace units
{
    class meters;
    class millimeters;
    class feet;
    class inches;

    class unit
    {
    public:

        typedef enum {m, mm, ft, in} unit_type;

        qreal value();

    protected:
        unit();
        qreal     _val;
        unit_type _type;

    friend QDebug operator<<(QDebug debug, units::unit val);
    };

    class meters : public unit
    {
    public:
        meters();
        meters(qreal val);
        meters(quint32 val);
        meters(meters& val);
        explicit meters(millimeters val);
        explicit meters(feet val);
        explicit meters(inches val);

        meters& operator=  (qint32 val);
        meters& operator=  (qreal val);
        meters& operator=  (meters val);
        meters& operator+  (meters val);
        meters& operator+= (meters val);
        meters& operator-  (meters val);
        meters& operator-= (meters val);
        //meters& operator+  (millimeters val);
        //meters& operator+= (millimeters val);
        //meters& operator-  (millimeters val);
        //meters& operator-= (millimeters val);

        friend class millimeters;
        friend class feet;
        friend class inches;
    };

    class millimeters : public unit
    {
    public:
        millimeters(qreal val);
        millimeters(quint32 val);
        millimeters(millimeters& val);
        explicit millimeters(meters val);
        explicit millimeters(feet val);
        explicit millimeters(inches val);

        millimeters& operator+ (millimeters val);
        millimeters& operator+= (millimeters val);
        millimeters& operator- (millimeters val);
        millimeters& operator-= (millimeters val);

        friend class meters;
        friend class feet;
        friend class inches;
    };

    class feet : public unit
    {
    public:
        feet(qreal val);
        feet(feet& val);
        explicit feet(meters val);
        explicit feet(millimeters val);
        explicit feet(inches val);

        feet& operator+ (feet val);
        feet& operator+= (feet val);
        feet& operator- (feet val);
        feet& operator-= (feet val);

        friend class meters;
        friend class millimeters;
        friend class inches;
    };

    class inches : public unit
    {
    public:
        inches(qreal val);
        inches(quint32 val);
        inches(inches& val);
        explicit inches(meters val);
        explicit inches(millimeters val);
        explicit inches(feet val);

        inches& operator+ (inches val);
        inches& operator+= (inches val);
        inches& operator- (inches val);
        inches& operator-= (inches val);

        friend class meters;
        friend class millimeters;
        friend class feet;
    };

    QDebug operator<<(QDebug debug, units::unit val)
    {
        QDebugStateSaver saver(debug);

        QString unit_text = "";
        switch (val._type)
        {
        case unit::m:
            unit_text = "m";
        break;

        case unit::mm:
            unit_text = "m";
        break;

        case unit::ft:
            unit_text = "m";
        break;

        case unit::in:
            unit_text = "m";
        break;
        }

        debug.nospace() << val._val << unit_text;
        return debug;
    }

};
