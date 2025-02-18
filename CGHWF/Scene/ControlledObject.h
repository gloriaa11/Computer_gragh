#pragma once

enum KEYBOARD_ACTIONS {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SWITCHSPOTLIGHT,
	MODE_SWITCH,
	TOGGLE_CLICK_MODE, // 添加模式切换的动作
	TOGGLE_VIEW_MODE   // 添加视角控制模式的切换
};

/*interface for controller*/
class ControlledObject {
public:
	virtual void ProcessMouseScroll(float yoffset)=0;
	virtual void ProcessMouseMovement(float xoffset, float yoffset)=0;
	virtual void ProcessKeyboard(KEYBOARD_ACTIONS direction,float delta_time)=0;
	virtual void ProcessKeyboard(KEYBOARD_ACTIONS action) = 0;
};
