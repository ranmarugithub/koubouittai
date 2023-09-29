﻿# include <Siv3D.hpp> // OpenSiv3D v0.6.11

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(Palette::Midnightblue);
	Texture submarine(U"⛵"_emoji);

	while (System::Update())
	{
		submarine.drawAt(400, 300);
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
