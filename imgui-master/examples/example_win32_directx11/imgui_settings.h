#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include <algorithm>
#include <string>

inline ImColor main_green(0, 255, 0);
inline ImColor main_red(255, 0, 0);
inline ImVec4 color_edit4 = ImColor(0, 110, 255, 255);
inline ImColor main_coCQlor(254, 249, 255);
namespace font
{
    inline ImFont* ContiB = nullptr;
    inline ImFont* icomoon = nullptr;
    inline ImFont* lexend_bold = nullptr;
    inline ImFont* lexend_general_bold = nullptr;
    inline ImFont* PoppinsRegular = nullptr;
    inline ImFont* icomoon_widget = nullptr;
    inline ImFont* icomoon_widget2 = nullptr;

}

namespace c {

    inline ImVec4 main_coCQlor1 = ImColor(0, 255, 0);

    inline ImVec4 child_rect = ImColor(15, 15, 17);
    inline ImVec4 logo_in_active = ImColor(145, 145, 160, 255);
    inline ImVec4 main_color3 = ImColor(13, 74, 167, 255);
    inline ImVec4 main_color4 = ImColor(13, 74, 167, 255);
    namespace elements
    {
       

        inline ImVec4 Circle = ImColor(99, 99, 99, 255);
        inline ImVec4 shadow = ImColor(255, 255, 255);

        inline ImVec4 mark = ImColor(255, 255, 255);

        inline ImVec4 stroke = ImColor(28, 26, 37);
        inline ImVec4 background = ImColor(15, 15, 17);
        inline ImVec4 background_widget = ImColor(21, 23, 26);

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hov = ImColor(81, 92, 109);
        inline ImVec4 text = ImColor(43, 51, 63);

        inline float rounding = 10;
    }

    inline ImVec4 child_bg = ImColor(14, 14, 14, 255);
    inline ImVec4 line = ImColor(33, 33, 33, 255);
    inline ImVec4 line_child = ImColor(30, 30, 30, 255);
    inline ImVec4 text_active = ImColor(255, 255, 255, 255);
    inline ImVec4 text_in_active_1 = ImColor(175, 175, 185, 255);
    inline ImVec4 text_in_active = ImColor(51, 51, 51, 255);
    inline ImVec4 logo_active = ImColor(255, 101, 87, 255);

    inline ImVec4 bg_checkbox = ImColor(10, 10, 10, 255);
    inline ImVec4 bg_checkbox_active = ImColor(73, 32, 28, 255);
    inline ImVec4 circle_active = ImColor(255, 101, 87, 255);
    inline ImVec4 circle_in_active = ImColor(36, 36, 37, 255);
    inline ImVec4 settings_checkbox = ImColor(54, 54, 54, 255);
    inline ImVec4 combo = ImColor(20, 20, 20, 255);
    inline ImVec4 rect_vehicle_in = ImColor(27, 27, 27, 255);
    inline ImVec4 rect_vehicle_active = ImColor(255, 101, 87, 255);
    inline ImVec4 bg_vehicle = ImColor(15, 15, 15, 255);
    inline ImVec4 circle_chk = ImColor(15, 15, 15, 255);

    inline ImVec4 separator = ImColor(30, 30, 30, 255);

    inline float rounding = 4;

    namespace input
    {
        inline ImVec4 stroke = ImColor(75, 76, 103, 255);
        inline ImVec4 line = ImColor(0, 10, 10, 200);
        inline float rounding = 2.f;
    }

    namespace text
    {
        inline ImVec4 text_active = ImColor(255, 255, 255, 255);
        inline ImVec4 text_hov = ImColor(128, 130, 170, 255);
        inline ImVec4 text = ImColor(75, 76, 103, 255);
    }
}

namespace OxcyColors {

    inline ImVec2 size = ImVec2(585, 500);

    inline ImVec4 text = ImColor(255, 255, 255, 255);
    inline ImVec4 icon_welcome = ImColor(255, 255, 255, 22);
    inline ImVec4 icon_user = ImColor(255, 255, 255, 148);
    inline ImVec4 text_notif = ImColor(153, 153, 153, 255);
    inline ImVec4 text_notif_description = ImColor(141, 151, 178, 216);
    inline ImVec4 text_1 = ImColor(255, 255, 255, 127);
    inline ImVec4 image_dota = ImColor(255, 255, 255, 10);
    inline ImVec4 text_logo = ImColor(227, 227, 227, 255);
    inline ImVec4 text_in = ImColor(77, 75, 75, 255);
    inline ImVec4 input_active = ImColor(255, 246, 180, 185);
    inline ImVec4 input_inactive = ImColor(255, 246, 180, 255);
    inline ImVec4 succes = ImColor(106, 255, 130, 255);
    inline ImVec4 image_bgs = ImColor(255, 255, 255, 9);
    // inline ImVec4 rect_multi = ImColor(153, 148, 108, 76);
    inline ImVec4 rect_multi_1 = ImColor(14, 15, 28, 165);
    inline ImVec4 rect_multi = ImColor(0, 0, 0, 153);
    inline ImVec4 rect_input = ImColor(12, 12, 12, 255);
    inline ImVec4 rect_input_1 = ImColor(14, 15, 28, 166);
    inline ImVec4 in_rect = ImColor(44, 44, 46, 186);

    inline ImVec4 text_page_login = ImColor(119, 101, 101, 193);

    inline ImVec4 accent = ImColor(255, 246, 180);
    inline ImVec4 outline = ImColor(48, 52, 65, 245);
    inline ImVec4 background = ImColor(30, 31, 36, 245);
    inline ImVec4 notif = ImColor(20, 20, 20, 120);
    inline ImVec4 color_bg = ImColor(2, 3, 9, 178);

    inline ImVec4 color_bg_tab = ImColor(0, 0, 0, 89);

    inline ImVec4 color_bg_child = ImColor(0, 0, 0, 143);

    inline ImVec4 color_rect_child = ImColor(14, 15, 28, 165);

    inline ImVec4 color_text_child = ImColor(255, 255, 255, 140);

    inline ImVec4 color_rect_tab = ImColor(27, 26, 26, 63);

    inline ImVec4 rect = ImColor(128, 100, 100, 76);

    inline ImVec4 rect_1 = ImColor(255, 255, 255, 15);

    inline ImVec4 color_bg_1 = ImColor(57, 56, 81, 51);

    inline ImVec4 main_yellow = ImColor(255, 246, 180, 255);

    inline ImVec4 line_tab = ImColor(48, 51, 76, 255);

    inline ImVec4 black_rect = ImColor(0, 0, 0, 51);

    inline ImVec4 game_line_tab = ImColor(25, 27, 47, 255);

    inline ImVec4 background_color = ImColor(0, 0, 0, 255);
    inline ImVec4 text_notifs = ImColor(209, 222, 255, 255);
    inline ImVec4 bg_spinner = ImColor(47, 47, 47, 140);

    inline ImVec4 undetect = ImColor(115, 255, 141, 51);
    inline ImVec4 undetect_text = ImColor(115, 255, 141, 255);

    inline ImVec4 color_teg_text = ImColor(255, 212, 102);
    inline ImVec4 color_teg_bg = ImColor(255, 210, 95, 79);

    inline ImVec4 color_teg_text_1 = ImColor(255, 95, 95);
    inline ImVec4 color_teg_bg_1 = ImColor(255, 54, 58, 51);

    inline ImVec4 circle = ImColor(255, 246, 180);

    inline ImVec4 circle_in = ImColor(24, 24, 27, 79);

    inline ImVec4 icon_tab_active = ImColor(0, 132, 255, 255);

    inline ImVec4 icon_tab_inactive = ImColor(28, 28, 28, 255);

    inline ImVec4 background_tab_active = ImColor(0, 57, 109, 153);

    inline ImVec4 background_tab_inactive = ImColor(12, 12, 12, 153);

    inline ImVec4 rect_tab_active = ImColor(0, 132, 255, 216);

    inline ImVec4 rect_tab_inactive = ImColor(17, 17, 17, 114);

    inline ImVec4 shadow_tab_active = ImColor(0, 132, 255, 255);

    inline ImVec4 shadow_tab_inactive = ImColor(0, 132, 255, 0);

    inline ImVec4 circle_icon = ImColor(0, 255, 38, 255);

    inline ImVec4 multi_checkbox_blue = ImColor(0, 132, 255, 255);

    inline ImVec4 multi_checkbox_black = ImColor(14, 15, 28, 140);

    inline ImVec4 child_bg = ImColor(3, 3, 3, 255);

    inline ImVec4 multi_checkbox_in = ImColor(17, 17, 17, 255);

    inline ImVec4 multi_checkbox_hover = ImColor(26, 26, 26, 255);

    inline ImVec4 circle_active = ImColor(0, 0, 0, 255);

    inline ImVec4 circle_inactive = ImColor(40, 40, 40, 255);
    inline ImVec4 circle_hover = ImColor(49, 49, 49, 255);
    inline ImVec4 rect_elements = ImColor(255, 255, 255, 3);

    inline ImVec4 rect_elements_2 = ImColor(16, 16, 16, 127);

    inline ImVec4 text_checkbox_active = ImColor(255, 255, 255, 142);
    inline ImVec4 text_checkbox_hover = ImColor(255, 255, 255, 204);
    inline ImVec4 text_checkbox_active_on = ImColor(255, 255, 255, 255);
    inline ImVec4 rect_notif = ImColor(255, 255, 255, 8);
    inline ImVec4 text_checkbox_inactive = ImColor(91, 91, 91, 255);

    inline ImVec4 text_checkbox_inactive_on = ImColor(211, 211, 211, 255);

    inline ImVec4 text_checkbox_inactive_hover = ImColor(149, 149, 149, 255);

    inline ImVec4 slider_multi_line = ImColor(0, 132, 255, 255);
    inline ImVec4 slider_multi_line_1 = ImColor(0, 0, 0, 255);
    inline ImVec4 slider_rect = ImColor(255, 255, 255, 255);
    inline ImVec4 slider_rect_in = ImColor(9, 9, 9, 255);

    inline ImVec4 combo_bg = ImColor(14, 14, 14, 165);
    inline ImVec4 combo_rect = ImColor(16, 16, 16, 127);
    inline ImVec4 combo_box = ImColor(18, 18, 18, 255);
    inline ImVec4 combo_bg_1 = ImColor(3, 3, 3, 255);
    inline ImVec4 combo_icon = ImColor(91, 91, 91, 255);
    inline ImVec4 combo_icon_active = ImColor(255, 255, 255, 140);

    inline ImVec4 selectable_bg = ImColor(9, 9, 9, 255);

    inline ImVec4 color_picker_multi_1 = ImColor(14, 15, 28, 255);
    inline ImVec4 color_picker_multi_2 = ImColor(1, 54, 104, 255);

    inline ImVec4 multi_child = ImColor(45, 45, 45, 0);
    inline ImVec4 multi_child_1 = ImColor(199, 199, 199, 15);
}

inline float anim_speed = 12.f;
inline ImColor GetColorWithAlpha(ImColor color, float alpha)
{
    return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
}


