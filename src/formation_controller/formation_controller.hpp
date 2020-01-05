#ifndef _FORMATION_CONTROL_HPP_
#define _FORMATION_CONTROL_HPP_
/*本程序的作用是提供几个类型下的编队控制器
例如只有GPS位置，有相对位置以及相对速度，有绝对位置以及绝对速度等
*/

#include <iostream>
#include"../fixed_wing_lib/tecs.hpp"
#include "../fixed_wing_lib/mathlib.hpp"
#include "../fixed_wing_lib/pid_controller.hpp"
#include "../fixed_wing_lib/vector.hpp"

using namespace std;

class FORMATION_CONTROL
{
public:
    struct _s_formation_controller_states //编队控制器内部的情况
    {
    };

    struct rel_states //领机从机相对状态
    {
        /* data */
    };

    struct _s_leader_states //领机状态信息
    {
        float pitch_angle{-20000}; //姿态只有姿态那个控制器才可能用到

        float yaw_angle{-20000};

        float roll_angle{-20000};

        float ned_vel_x{-20000}; //由于NED以及GPS坐标系系均为惯性系，两个速度是一致的

        float ned_vel_y{-20000};

        float ned_vel_z{-20000};

        double latitude{-20000};

        double longtitude{-20000};

        double altitude{-20000};

        float relative_alt{-20000};

        float air_speed{-20000};
    };

    struct _s_fw_states //本机状态信息
    {
        float pitch_angle{-20000};

        float yaw_angle{-20000};

        float roll_angle{-20000};

        float ned_vel_x{-20000}; //由于NED以及GPS坐标系系均为惯性系，两个速度是一致的

        float ned_vel_y{-20000};

        float ned_vel_z{-20000};

        double latitude{-20000};

        double longtitude{-20000};

        double altitude{-20000};

        float relative_alt{-20000};

        float air_speed{-20000};

        float wind_estimate_x{-20000};

        float wind_estimate_y{-20000};

        float wind_estimate_z{-20000};
    };

    struct _s_fw_error //本机误差
    {
        //体轴系位置误差<与自己期望>
        float PXb{0};
        float PYb{0};
        float PZb{0};

        //体轴系速度误差<与自己期望>
        float VXb{0};
        float VYb{0};
        float VZb{0};

        //体轴系速度误差<与领机>
        float led_fol_vxb{0};
        float led_fol_vyb{0};
        float led_fol_vzb{0};
    };

    struct _s_4cmd
    {
        float roll{0};
        float pitch{0};
        float yaw{0};
        float thrust{0};
    };

    void set_formation_type(int formation_type); //设定编队形状

    void reset_formation_controller(); //重置控制器，防止不同阶段控制器的状态混乱。

    _s_4cmd get_formation_4cmd(); //得到编队控制后的四通道控制量

    //几个编队控制器类型,根据能得到的领机信息分类

    //得到领机的位置，速度，姿态
    void att_vel_pos_controller();

    //得到领机的绝对位置，绝对速度
    void abs_pos_vel_controller(FORMATION_CONTROL::_s_leader_states leader_states,
                                FORMATION_CONTROL::_s_fw_states fw_states);

    //得到领机的仅仅有绝对位置
    void abs_pos_controller();

    //得到领机的相对位置与相对速度
    void rel_pos_vel_controller();

    //得到领机的相对位置
    void rel_pos_controller();

private:
    bool rest_speed_pid{false};
    struct _s_formation_offset
    {
        //机体系
        float xb{0};
        float yb{0};
        float zb{0};

        //NED系
        float ned_n{0};
        float ned_e{0};
        float ned_d{0};
    } formation_offset;

    struct _s_fw_sp
    {
        float pitch_angle{-20000};

        float yaw_angle{-20000};

        float roll_angle{-20000};

        float ned_vel_x{-20000};

        float ned_vel_y{-20000};

        float ned_vel_z{-20000};

        double latitude{-20000};

        double longtitude{-20000};

        double altitude{-20000};

        float relative_alt{-20000};

        float air_speed{-20000};

        float ground_speed{-20000};
    } fw_sp;

    struct _s_formation_params
    {
        float kv_p{0.5}; //主从机速度差比例项

        float kp_p{0.8}; //从机期望与实际位置误差比例

        float mix_kp{0.6};

        float mix_kd{0.0};

        float mix_ki{0.01};
    } formation_params;

    _s_fw_error fw_error;

    Point get_plane_to_sp_vector(Point origin, Point target);
};

#endif