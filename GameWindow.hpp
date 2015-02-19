#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP
#include <alc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/detail/type_mat.hpp>
#include "Platform.hpp"
#include "ui/Manager.hpp"

using std::shared_ptr;

namespace Diggler {

class Game;
class State;

class GameWindow {
private:
	static int InstanceCount;
	static bool IsGlewInited;
	
	GLFWwindow *m_window;
	int m_w, m_h;
	
	shared_ptr<State> m_currentState, m_nextState;

public:
	UI::Manager UIM;
	
	Game *G;
	
	GameWindow();
	~GameWindow();
	
	operator GLFWwindow&() const { return *m_window; }
	operator GLFWwindow*() const { return m_window; }
	
	inline int getW() const { return m_w; }
	inline int getH() const { return m_h; }
	
	void cbMouseButton(int key, int action, int mods);
	void cbCursorPos(double x, double y);
	void cbMouseScroll(double x, double y);
	void cbKey(int key, int scancode, int action, int mods);
	void cbChar(char32 unichar);
	void cbResize(int w, int h);
	
	void updateViewport();
	
	void setNextState(const shared_ptr<State> next);
	void run();
	
	void showMessage(const std::string &msg, const std::string &submsg = "");
};

}

#endif