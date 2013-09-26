#ifndef STRUCTS_H
#define STRUCTS_H

struct __settings {

    struct {
        quint8  functions;
        quint8  cut_type;
        quint16 sensor_threshold;
        quint16 slip_threshold;
        quint8  sensor_gain;
        quint8  sensor_direction;
        quint16 min_speed;
        quint16 min_rpm;
        quint16 gears_ratio[6];
        quint8  gears_cut_time[5];
        quint8  tc_gear_trim[6];
    } data;
    quint32 CRCValue;
};
typedef struct __settings settings_t;

struct __status {
    quint8 shifting:1;
    quint8 slipping:1;
    qint16 slipping_pct;
    quint8 acceleration;
};
typedef struct __status status_t;

struct __light_settings {

    quint8 state; /* Light state (Blink?) */
    quint16 duration; /* Duration/interval in ms */
};
typedef struct __light_settings light_settings_t;

struct __sensors {
    quint16 rpm;
    quint16 speed;
    quint16 strain_gauge;
    quint16 tc_switch;
    quint16 vbat;
};
typedef struct __sensors sensors_t;

#endif // STRUCTS_H
