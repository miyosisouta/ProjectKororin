#pragma once

/* テキストの設定時に必要な情報 */
struct MessageType
{
	/* メッセージシーン */
	enum MessageScene
	{
		GameStart,		// ゲーム開始時の会話
		SphereLevel2,	// 塊の大きさが2段階目の時の会話
		SphereLevel3,	// 塊の大きさが3段階目の時の会話
		GameFailure		// クリア失敗時の会話
	};

	/* 言語 */ 
	enum Language
	{
		ja,		//!< 日本語
		en,		//!< 英語
		max,	//!< 対応言語
	};
};


static std::vector<std::wstring> sMessageGameStartTextJaList = {
	std::wstring(L"ヤア、オキマシタカ。オハヨウ、\nアルイハコンバンワ。ドチラデモヨイデスネ"),
	std::wstring(L"ジツハサクヤ、ワガハイガスコシハシャギスギテ、\nヨゾラノホシヲイクツカケシトバシテシマイマシテネ。"),
	std::wstring(L"宇宙（コスモ）ガスカスカデ、ナンダカサビシイデスヨ。ソコデアナタノデバンデス。"),
	std::wstring(L"ソノヘンニアルモノヲテキトウニ、\nカツジョウネツテキニマキコンデ、\nアタラシイホシヲツクリナサイ。"),
	std::wstring(L"チイサナモノカラコツコツト、\nデスガダイタンニ。"),
	std::wstring(L"サア、ナニヲボーットシテイルノデス？\nハヤクコロガシニイキナサイ。")
};
static std::vector<std::wstring> sMessageGameStartTextEnList = {
	std::wstring(L"Hi, are you awake?\n Good morning or good evening.\nEither is fine."),
	std::wstring(L"Actually, last night I got a little too\nexcited and ended up blowing out\na few stars in the night sky."),
	std::wstring(L"The universe is empty and lonely.\nThat's where you come in."),
	std::wstring(L"Create a new planet by incorporating whatever\nis around you in a suitable and passionate way."),
	std::wstring(L"Start with small things,\nbut be bold."),
	std::wstring(L"So what are you doing daydreaming?\nGet rolling!"),
};



static std::vector<std::wstring> sMessageSphereLevelSecondTextJaList = {
	std::wstring(L"フム…スコシハカタチニナッテキタヨウデスネ。"),
	std::wstring(L"タトエルナラ、ミチバタニオチテイルオニギリ…\nイヤ、テニスボールトイッタトコロデショウカ。"),
	std::wstring(L"ワルクハナイデスガ、マダマダ「ホシ」トヨブニハハズカシイ、ヒンソウナサイズデス。"),
	std::wstring(L"ソロソロ、ソノヘンノカビンヤ、\nニワサキノキクライハマキコメルノデハ？"),
	std::wstring(L"エンリョハイリマセンヨ、ヒメイモドゴウモ、\nスベテハウツクシイホシノスパイス。"),
	std::wstring(L"モットドンヨクニ、ガツガツトイキナサイ。"),
};
static std::vector<std::wstring> sMessageSphereLevelSecondTextEnList = {
	std::wstring(L"Hmm... \nit looks like it's starting to take shape a little."),
	std::wstring(L"To put it in perspective, \nit's like a rice ball found on the side of the road...\nor maybe a tennis ball."),
	std::wstring(L"It's not bad, \nbut it's still too small to be called a star."),
	std::wstring(L"Maybe it'll soon be able to wrap up\nsome nearby vases or trees in the garden?"),
	std::wstring(L"Don't be shy. Screams and shouts are \nall part of the spice of a beautiful planet."),
	std::wstring(L"Be more greedy and go for it."),
};



static std::vector<std::wstring> sMessageSphereLevelThirdTextJaList = {
	std::wstring(L"オオッ！スバラシイ！\nダイブタクマシクソダッテキマシタネェ！"),
	std::wstring(L"ココマデクルト、コロガルシンドウガ\nワガハイノアシモトマデツタワッテキマスヨ。"),
	std::wstring(L"マチノヒトビトガニゲマドウスガタ…\nン～、ナントイトオシイカオスナコウケイデショウ。"),
	std::wstring(L"モウガードレールヤカンバンニツマズクコトモナイハズデス。"),
	std::wstring(L"サア、ココカラハ\nハカイトソウゾウノパレードデス！\nイエモ、クルマモ、デンチュウモ！"),
	std::wstring(L"メニツクスベテヲマキコンデ、\nワガハイヲモットカンドウサセテゴランナサイ！"),
};
static std::vector<std::wstring> sMessageSphereLevelThirdTextEnList = {
	std::wstring(L"Wow! That's amazing!\nYou've grown so strong!"),
	std::wstring(L"At this point, \nI can feel the vibrations of the rolling\nall the way down to my feet."),
	std::wstring(L"The sight of people\nin the city fleeing in panic...\nwhat a lovely chaotic scene."),
	std::wstring(L"You'll never trip over guardrails or signs again."),
	std::wstring(L"Now, let's start the parade\nof destruction and creation!\n Houses, cars, and telephone poles!"),
	std::wstring(L"Get everything you see \ninvolved and impress me even more!"),
};



static std::vector<std::wstring> sMessageGameFailureTextJaList = {
	std::wstring(L"ナ、ナンデスカ、コノチンケナカタマリハ、\nオウサマノマエニハ、ホコリニシカミエマセンヨ。"),
	std::wstring(L"コンナヒンソウナモノ、\nヨゾラニウカベタラホシクズタチガ、\nワライシンデシマイマス。"),
	std::wstring(L"オウサマ、アナタノアマリノフガイナサニ、\nハートガキリキリイタミダシテキマシタ。"),
	std::wstring(L"カオモミタクアリマセン。\nハンセイベヤデ、\nソノマメツブノヨウナカタマリトイッショニ、\nオシクラマンジュウデモシテナサイ。"),
	std::wstring(L"サッサトイッテ、\nツギハモットデッカイユメトキボウヲ\nマキコンデクルンデスヨ!!\n喝ッ!!!"),
};
static std::vector<std::wstring> sMessageGameFailureTextEnList = {
	std::wstring(L"For some reason, \nthis little thing...\ncannot be seen in front of the king."),
	std::wstring(L"If something like this were to happen, \nit would be a disaster if it were to be left alone."),
	std::wstring(L"King, \nI was so surprised at your wonder\n that my heart was torn apart."),
	std::wstring(L"I don't have a face. \nPlease wash it with the same amount of water as the bean bags."),
	std::wstring(L"Hurry up!! \nlet's make even bigger dreams and hopes come true next time! ... \nCheers!!"),
};



static const wchar_t* GetMessageText(int index, MessageType::MessageScene scene, MessageType::Language languageType = MessageType::Language::ja)
{
	switch (scene)
	{
	case MessageType::GameStart:
	{
		if (languageType == MessageType::ja) {
			return sMessageGameStartTextJaList[index].c_str();
		}
		return sMessageGameStartTextEnList[index].c_str();

	}
	case MessageType::SphereLevel2:
	{
		if (languageType == MessageType::ja) {
			return sMessageSphereLevelSecondTextJaList[index].c_str();
		}
		return sMessageSphereLevelSecondTextEnList[index].c_str();

	}
	case MessageType::SphereLevel3:
	{
		if (languageType == MessageType::ja) {
			return sMessageSphereLevelThirdTextJaList[index].c_str();
		}
		return sMessageSphereLevelThirdTextEnList[index].c_str();
	}
	case MessageType::GameFailure:
	{
		if (languageType == MessageType::ja)
		{
			return sMessageGameFailureTextJaList[index].c_str();
		}
		return sMessageGameFailureTextEnList[index].c_str();
	}
	default:
	{
		break;
	}
	}
};