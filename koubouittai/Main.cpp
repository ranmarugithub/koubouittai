﻿# include <Siv3D.hpp>

//シーンの名前
enum class State
{
	Title,
	Game,
	Clear,
	GameOver,
};

using App = SceneManager<State>;

// タイトルシーン

class Title : public App::Scene
{
public:

	const Texture shinkai{ U"example/sinkai.jpg" };

	// コンストラクタ（必ず実装）
	Title(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		m_startTransition.update(m_startButton.mouseOver());

		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.leftClicked())
		{
			// ゲームシーンへ
			changeScene(State::Game, 0.5s);
		}
		else if (m_exitButton.leftClicked())
		{
			// 終了
			System::Exit();
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		shinkai.drawAt(400, 300);

		FontAsset(U"TitleFont")(U"攻防一体")
			.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

		FontAsset(U"Menu")(U"START GAME").drawAt(m_startButton.center(), ColorF{ 0.25 });
		FontAsset(U"Menu")(U"EXIT").drawAt(m_exitButton.center(), ColorF{ 0.25 });
	}

private:

	Rect m_startButton{ Arg::center = Scene::Center(), 300, 60 };
	Transition m_startTransition{ 0.4s, 0.2s };

	Rect m_exitButton{ Arg::center = Scene::Center().movedBy(0, 100), 300, 60 };
	Transition m_exitTransition{ 0.4s, 0.2s };
};

class Game : public App::Scene
{
public:

	const Texture monster{ U"example/monster.png" };
	const Texture submarine{ U"example/sensuikan.png" };

	Vec2 GenerateEnemy()
	{
		return RandomVec2(Rect{-400, -400, 1200, 1000});
	}
	//画面の大きさ
	const RectF shape{ 0, 0, 800, 600 };

	Circle player;
	Circle shot;
	Array<Circle> enemies;
	Array<bool> enemiesEnable;

	//自機
	Vec2 playerPos{ 400,300 };
	bool playerEnable = true;

	//弾
	Vec2 shotpos{ 400, 300 };
	bool shotEnable = false;
	double directionx = 0.0;
	double directiony = 0.0;
	const double speed = 200;

	//敵の発生速度
	int32 cooltime = 200;

	const Font font{ FontMethod::MSDF,48 };
	double timeLeft = 30.0;

	// コンストラクタ（必ず実装）
	Game(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		ClearPrint();

		const double deltaTime = Scene::DeltaTime();
		//playerの位置
		player = { playerPos,30 };
		//弾の位置
		shot = { shotpos,20 };

		//敵を発生させる
		--cooltime;
		if (cooltime <= 0)
		{
			Vec2 pos = GenerateEnemy();
			if (pos.x < 0 || pos.x > Scene::Width() || pos.y < 0 || pos.y > Scene::Height())
			{
				enemies << Circle(pos, 20);
				enemiesEnable << true;
			}
			cooltime = Random(150, 300);
		}

		//敵の移動

		for (auto i : step(enemies.size()))
		{
			if (enemiesEnable[i] == true)
			{
				enemies[i].x += (playerPos.x - enemies[i].x) * 0.005 * Random(1,2);
				enemies[i].y += (playerPos.y - enemies[i].y) * 0.005;
			}
		}

		//弾を発射
		if (KeySpace.down())
		{
			shotEnable = true;
		}

		//弾の移動入力
		// 
		// ←
		if (KeyA.pressed())
		{
			if (directionx > -1.0)
			{
				directionx -= 0.01;
			}
			if (directiony > 0)
			{
				directiony -= 0.005;
			}
			if (directiony < 0)
			{
				directiony += 0.005;
			}
		}

		// → 
		if (KeyD.pressed())
		{
			if (directionx < 1.0)
			{
				directionx += 0.01;
			}
			if (directiony > 0)
			{
				directiony -= 0.005;
			}
			if (directiony < 0)
			{
				directiony += 0.005;
			}
		}

		// ↑ 
		if (KeyW.pressed())
		{
			if (directiony > -1.0)
			{
				directiony -= 0.01;
			}
			if (directionx > 0)
			{
				directionx -= 0.005;
			}
			if (directionx < 0)
			{
				directionx += 0.005;
			}
		}

		// ↓ 
		if (KeyS.pressed())
		{
			if (directiony < 1.0)
			{
				directiony += 0.01;
			}
			if (directionx > 0)
			{
				directionx -= 0.005;
			}
			if (directionx < 0)
			{
				directionx += 0.005;
			}
		}

		//弾の発射位置
		if (shotEnable == false)
		{
			// ←
			if (KeyA.pressed())
			{
				shotpos.x = playerPos.x - 41;
				shotpos.y = 300;
			}
			// → 
			if (KeyD.pressed())
			{
				shotpos.x = playerPos.x + 41;
				shotpos.y = 300;
			}
			// ↑ 
			if (KeyW.pressed())
			{
				shotpos.y = playerPos.y - 41;
				shotpos.x = 400;
			}
			// ↓ 
			if (KeyS.pressed())
			{
				shotpos.y = playerPos.y + 41;
				shotpos.x = 400;
			}
		}
		
		if (shotEnable == true)
		{
			//弾の移動
			shotpos.x += (speed * deltaTime * directionx);
			shotpos.y += (speed * deltaTime * directiony);

			//画面外に出たら弾を無効にする
			if (shotpos.x < 0 || shotpos.x > 800 || shotpos.y < 0 || shotpos.y > 600)
			{
				shotEnable = false;
				shotpos = { 400, 300 };
				directionx = 0;
				directiony = 0;
			}
		}
		//当たり判定

		for (auto i : step(enemies.size()))
		{
			if (enemiesEnable[i] == true)
			{
				//敵vs弾
				if (shotEnable == true)
				{
					if (shot.intersects(enemies[i]))
					{

						shotEnable = false;
						shotpos = { 400, 300 };
						directionx = 0;
						directiony = 0;

						enemiesEnable[i] = false;
					}
				}
				//敵vs自機
				if (player.intersects(enemies[i]))
				{
					playerEnable = false;
					enemiesEnable[i] = false;
				}

			}
		}
		//弾vs自機
		if (shot.intersects(player) && shotEnable == true)
		{
			playerEnable = false;
		}

		//クリア判定
		timeLeft -= Scene::DeltaTime();
		if (timeLeft <= 0)
		{
			changeScene(State::Clear, 0.5s);
		}
		//ゲームオーバー判定
		if (playerEnable == false)
		{
			changeScene(State::GameOver, 0.5s);
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		//背景
		Scene::SetBackground(Palette::Midnightblue);
		Rect{ 0,0,800,600 }.draw(Arg::top = Palette::Midnightblue, Arg::bottom = Palette::Black);

		//player.draw(Palette::White);
		if (shotEnable == true)
		{
			shot.draw(Palette::Yellow);
		}
		submarine.scaled(0.2).drawAt(playerPos);

		for (auto i : step(enemies.size()))
		{
			if (enemiesEnable[i] == true)
			{
				enemies[i].draw(Palette::Red);
				monster.scaled(0.1).drawAt(enemies[i].x, enemies[i].y);
			}
		}

		if (0.0 < timeLeft)
		{
			font(U"残り時間 {:.2f}"_fmt(timeLeft)).draw(30, 10, 10, Palette::White);
		}
		else
		{

		}
	}
};

// ゲームクリアシーン
class Clear : public App::Scene
{
public:

	const Font fonta{ FontMethod::MSDF,48 };

	const Texture yudati{ U"example/taiyo.jpg" };

	// コンストラクタ（必ず実装）
	Clear(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		m_clearTransition.update(m_clearButton.mouseOver());
		if (m_clearButton.leftClicked())
		{
			// タイトルシーンへ
			changeScene(State::Title, 0.5s);
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		yudati.scaled(1.0).drawAt(400, 300);

		fonta(U"GAME CLEAR").draw(120, 20, 100, Palette::Gold);

		m_clearButton.draw(ColorF{ 1.0, m_clearTransition.value() }).drawFrame(2);

		FontAsset(U"Menu")(U"BACK TITLE").drawAt(m_clearButton.center(), ColorF{ 0.25 });

	}

private:
	Rect m_clearButton{ Arg::center = Scene::Center().movedBy(0, 50), 300, 60 };
	Transition m_clearTransition{ 0.4s, 0.2s };
};

// ゲームオーバーシーン
class GameOver : public App::Scene
{
public:

	const Font fonta{ FontMethod::MSDF,48 };

	const Texture deepsea{ U"example/DSC_9317.jpg" };

	// コンストラクタ（必ず実装）
	GameOver(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		m_gameOverTransition.update(m_gameOverButton.mouseOver());
		if (m_gameOverButton.leftClicked())
		{
			// タイトルシーンへ
			changeScene(State::Title, 0.5s);
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		deepsea.scaled(1.0).drawAt(400, 300);

		fonta(U"GAME OVER").draw(100, 85, 100, Palette::Red);

		m_gameOverButton.draw(ColorF{ 0.0, m_gameOverTransition.value() }).drawFrame(2);

		FontAsset(U"Menu")(U"BACK TITLE").drawAt(m_gameOverButton.center(), ColorF{ 1.0 });
	}
private:
	Rect m_gameOverButton{ Arg::center = Scene::Center().movedBy(0, 50), 300, 60 };
	Transition m_gameOverTransition{ 0.4s, 0.2s };
};

void Main()
{
	FontAsset::Register(U"TitleFont", 75, Typeface::Regular);
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);

	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は "Title"）を登録
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Clear>(State::Clear);
	manager.add<GameOver>(State::GameOver);

	while (System::Update())
	{
		// 現在のシーンを実行
		// シーンに実装した .update() と .draw() が実行される
		if (not manager.update())
		{
			break;
		}
	}
}
