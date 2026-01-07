#pragma once

/* テキストの設定時に必要な情報 */
struct MessageType
{
	/* メッセージシーン */
	enum MessageScene
	{
		GameStart,			// ゲーム開始時の会話
		SphereLevel2,		// 塊の大きさが2段階目の時の会話
		SphereLevel3,		// 塊の大きさが3段階目の時の会話
		GameClear_Good,		// ゲームクリア時の最高評価の会話
		GameClear_Normal,	// ゲームクリア時の通常の会話
		GameFailure			// クリア失敗時の会話
	};

	/* 言語 */ 
	enum Language
	{
		ja,		//!< 日本語
		en,		//!< 英語
		max,	//!< 対応言語数
	};
};

/* ゲームスタート時のテキスト */
static std::vector<std::wstring> sMessageGameStartTextJaList = {
	std::wstring(L"コノヨゴレタセカイヲ、\nアルベキウツクシイスガタニモドス。\nソレダケガ、ワレワレノシメイダ。"),
	std::wstring(L"チラカッタモノハ、タダノノイズダ。\nスベテヲヒトツニマトメテ、\nカンペキナチツジョヲウミダセ。"),
	std::wstring(L"ワタシガミタイノハ、\nゴミヒトツナイカンゼンナルクウハクダ。\nメザワリナモノハ、スベテカタヅケロ。"),
	std::wstring(L"ヨゴレタカンキョウハ、\nヒトノココロマデムシバム。\nテッテイテキナジョウカデ、\nセカイヲスクウノダ。"),
	std::wstring(L"コノヨニフヨウナモノナドナイ。\nアルノハ、オカレルバショヲマチガエタ、\nシゲンタチダケダ"),
};
static std::vector<std::wstring> sMessageGameStartTextEnList = {
	std::wstring(L"To restore this polluted world\nto its original beauty.\nThat is our solitary mission."),
	std::wstring(L"Scattered objects are nothing but noise.\nGather everything into one\nand create perfect order."),
	std::wstring(L"What I desire to see is\na perfect void with no trash.\nClean up every single eyesore."),
	std::wstring(L"A filthy environment rots\neven the human heart.\nSave the world through thorough purification."),
	std::wstring(L"There is nothing unnecessary in this world.\nThere are only resources\nthat have been placed in the wrong spot."),
};


/* 塊のサイズレベルが2になった時のテキスト */
static std::vector<std::wstring> sMessageSphereLevelSecondTextJaList = {
	std::wstring(L"フム、スコシハカタチニナッテキタヨウデスネ。"),
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


/* 塊のサイズレベルが3になった時のテキスト */
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


/* ゲームクリアしたときの最高評価テキスト */
static std::vector<std::wstring> sMessageGameClearGoodTextJaList = {
	std::wstring(L"コレコソガ、\nワタシガユメミタソウジカクメイソノモノダ！\nコノウツクシイアッシュクニワカンドウサエオボエルゾ。"),
	std::wstring(L"カンペキダ、\nコノエリアノゴミヲスベテノミコミ、\nヒトツノゲイジュツエトショウカサセタシゴトブリワ、\nマサニモハンテキダ。"),
	std::wstring(L"スバラシイ！\nコノアツマルベキシテアツマッタゴミノケッショウワ\nワガソシキノシンボルトシテ\nゲンカンニカザリタイクライダ。"),
	std::wstring(L"ミヨ！コノアットウテキナシツリョウコソガ\nワタシタチガモトメテイタ\nシンノクリーンカンキョウノスガタデワナイカ！"),
	std::wstring(L"ソウジノガイネンヲクツガエス\nキョウイテキナセイカダ！\nキミノヨウナジンザイガイタコトヲ\nワタシワホコリニオモウゾ！"),
};


static std::vector<std::wstring> sMessageGameClearGoodTextEnList = {
	std::wstring(L"This is exactly the cleaning revolution\nI dreamed of!\nI am actually moved by this\nbeautiful compression."),
	std::wstring(L"Perfection...\nYou swallowed every bit of trash in this area.\nTransforming it into art is\ntruly exemplary work."),
	std::wstring(L"Wonderful!\nThis crystal of trash, gathered as it should be,\ndeserves to be displayed in our entrance\nas a symbol of our organization."),
	std::wstring(L"Behold! This overwhelming mass is\nthe true form of the clean environment\nwe have been seeking!"),
	std::wstring(L"A miraculous result that overturns\nthe very concept of cleaning!\nI am proud to have a talent like you\non the team!"),
};


/* ゲームクリアしたときの通常テキスト */
static std::vector<std::wstring> sMessageGameClearNormalTextJaList = {
	std::wstring(L"マア、ワルクハナイ\nケイカクドオリノスウチハダセテイルガ、\nソコニワタシタチノリネンエノジョウネツハ、\nカンジラレナイナ。"),
	std::wstring(L"タシカニキレイニハナッタガ、\nタダゴミヲアツメタダケト\nイワレテモヒテイデキナイ、\nヘイボンナシアガリトイワザルヲエナイ。"),
	std::wstring(L"キテイノサイズハクリアシテイルガ、\nキミナラモットダイタンニ、\nエリアゼンタイヲマキコムコトガデキタノデハナイカ？"),
	std::wstring(L"サギョウトシテハゴウカクダガ、\nコレデハマダソウジガ、\nヒトビトノココロヲウゴカス\nエンターテインメントニハナリエテイナイナ。"),
	std::wstring(L"コンカイハコレデヨシトシヨウ。\nダガツギハ、モットゴミタチガヨロコンデクッツクヨウナ、\nミリョクテキナカタマリヲキタイスルゾ。"),
};
static std::vector<std::wstring> sMessageGameClearNormalTextEnList = {
	std::wstring(L"Well, it is not bad.\nYou hit the target numbers,\nbut I do not feel any passion\nfor our philosophy in it."),
	std::wstring(L"It is certainly clean,\nbut I cannot deny it looks like\nyou just gathered trash.\nI must call this a mediocre finish."),
	std::wstring(L"You cleared the required size,\nbut surely you could have been bolder?\nWhy not roll up the entire area?"),
	std::wstring(L"As a task, it is a passing grade.\nBut this has not yet become entertainment\nthat moves people's hearts."),
	std::wstring(L"I will accept this for now.\nBut next time, I expect an attractive clump\nthat trash will happily stick to."),
};


/* ゲームクリアに失敗したときのテキスト */
static std::vector<std::wstring> sMessageGameFailureTextJaList = {
	std::wstring(L"ナンダコレハ？\nソウジヲシタトイウヨリハ、\nタダゴミヲチラカシテ\nアソンデイタダケノヨウニシカミエナイノダガ？"),
	std::wstring(L"アマリニモチイサイ。\nコンナヒンキュウナカタマリデハ、\nワタシタチノソウジプロジェクトノ\nメンボクガマルツブレデハナイカ！"),
	std::wstring(L"ハナシニナラナイナ。\nコンナチュウトハンパナ\nシゴトデキレイニシタツモリナラ、\nイマスグジシンノイシデヤリナオシテキタマエ。"),
	std::wstring(L"ワタシガキタイシテイタノハ、\nコンナソアクナデキノカタマリデハナイ！\nゴミエノアイジョウガ\nマッタクタリテイナイショウコダ。"),
	std::wstring(L"コレヲモッテ\nソウジカンリョウトイイハルツモリカ？\nメガフシアナデナケレバ、\nマダソコラジュウニゴミガアルノガミエルダロウ！"),
};
static std::vector<std::wstring> sMessageGameFailureTextEnList = {
	std::wstring(L"What is the meaning of this?\nRather than cleaning,\nit looks like you were just scattering trash\naround and playing with it?"),
	std::wstring(L"It is far too small...\nWith such a poor clump,\nthe reputation of our Cleaning Project\nis completely ruined!"),
	std::wstring(L"Out of the question.\nIf you intend to clean with such half-hearted work,\ngo redo it immediately\non your own will."),
	std::wstring(L"This is not what I expected!\nThis shoddy clump is proof that\nyou lack any love for trash whatsoever."),
	std::wstring(L"Do you intend to claim\nthe cleaning is complete with this?\nUnless you are blind,\nyou can still see trash everywhere!"),
};


/**
 * @brief シーンごとのテキストの入手
 * @param index 文の数
 * @param scene シーン
 * @param languageType 言語
 * @return 
 */
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
	case MessageType::GameClear_Good:
	{
		if (languageType == MessageType::ja) {
			return sMessageGameClearGoodTextJaList[index].c_str();
		}
		return sMessageGameClearGoodTextEnList[index].c_str();
	}
	case MessageType::GameClear_Normal:
	{
		if (languageType == MessageType::ja) {
			return sMessageGameClearNormalTextJaList[index].c_str();
		}
		return sMessageGameClearNormalTextEnList[index].c_str();
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