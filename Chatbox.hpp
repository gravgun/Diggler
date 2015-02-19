#ifndef CHATBOX_HPP
#define CHATBOX_HPP
#include <chrono>
#include <list>
#include <glm/glm.hpp>
#include "Platform.hpp"
#include "ui/Text.hpp"

namespace Diggler {

class Program;

class Game;

class Chatbox {
public:
	typedef std::chrono::system_clock system_clock;
	typedef std::chrono::time_point<system_clock> time_point;

private:
	static const Program *RenderProgram;
	static GLint RenderProgram_coord, RenderProgram_color, RenderProgram_mvp;
	
	bool m_isChatting;
	Game *G;
	struct ChatEntry {
		time_point date;
		int height;
		UI::Text *text;
		
		~ChatEntry();
	};
	std::list<ChatEntry> m_chatEntries;
	// TODO: Update when libstdc++ supports locale codecvt facets
	//std::u32string m_chatString;
	std::string m_chatString;
	UI::Text *m_chatText;
	struct Vertex {
		float x, y, r, g, b, a;
	};
	VBO m_vbo;
	int m_posX, m_posY;

public:
	Chatbox(Game *G);
	~Chatbox();
	
	bool isChatting() const;
	void setIsChatting(bool value);
	
	void addChatEntry(const std::string& text);
	
	void handleChar(char32 unichar);
	void handleKey(int key, int scancode, int action, int mods);
	
	void setPosition(int x, int y);
	
	void render();
	
	std::string getChatString() const { return m_chatString; }
};

}

#endif