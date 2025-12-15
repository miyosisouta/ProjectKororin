#pragma once


struct MessageLanguageType
{
	enum Enum
	{
		ja,
		en,
		max,
	};
};


static std::vector<std::wstring> sMessageTextList = {
	std::wstring(L"ナ、ナンデスカ、コノチンケナカタマリハ、\nオウサマノマエニハ、ホコリニシカミエマセンヨ。"),
	std::wstring(L"コンナヒンソウナモノ、ヨゾラニウカベタラホシクズタチガ、ワライシンデシマイマス。"),
	std::wstring(L"オウサマ、アナタノアマリノフガイナサニ、ハートガキリキリイタミダシテキマシタ。"),
	std::wstring(L"カオモミタクアリマセン。ハンセイベヤデ、ソノマメツブノヨウナカタマリトイッショニ、オシクラマンジュウデモシテナサイ。"),
	std::wstring(L"サッサトイッテ、ツギハモットデッカイユメトキボウヲマキコンデクルンデスヨ！喝ッ！！"),
};


static std::vector<std::wstring> sMessageTextEnList = {
	std::wstring(L"For some reason, this little thing...cannot be seen in front of the king."),
	std::wstring(L"If something like this were to happen, it would be a disaster if it were to be left alone."),
	std::wstring(L"King, I was so surprised at your wonder that my heart was torn apart."),
	std::wstring(L"I don't have a face. Please wash it with the same amount of water as the bean bags."),
	std::wstring(L"Hurry up and let's make even bigger dreams and hopes come true next time! ... Cheers!!"),
};


static const wchar_t* GetMessageText(int index, MessageLanguageType::Enum languageType = MessageLanguageType::ja)
{
	if (languageType == MessageLanguageType::ja) {
		return sMessageTextList[index].c_str();
	}
	return sMessageTextEnList[index].c_str();
}