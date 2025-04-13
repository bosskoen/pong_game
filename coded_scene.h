#pragma once

namespace Util {
	/// <summary>
	/// difuculty:
	/// 1 = ezze
	/// 2 = medeum
	/// 3 = hard
	/// 4 = imposeble
	/// </summary>
	void load_pong(bool with_AI = true, bool with_mouce = true, int difuculty = 1);

	void load_pong_menu();

	void load_win_pong(bool left_won, bool was_AI);
}