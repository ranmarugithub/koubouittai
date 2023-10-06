# include <Siv3D.hpp>

//シーンの名前
enum class State
{
	Title,
	Game,
	Clear,
};

using App = SceneManager<State>;

// タイトルシーン

class Title : public App::Scene
{
public:

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
		Rect{ 0,0,800,600 }.draw(Arg::top = Palette::Deepskyblue, Arg::bottom = Palette::Midnightblue);

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

	// コンストラクタ（必ず実装）
	Game(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		//画面の大きさ
		const RectF shape{ 0, 0, 800, 600 };

		Circle shot;
		Circle enemy;

		Vec2 shotpos{ 400, 300 };

		//敵の初期位置ランダム
		Vec2 enemypos = RandomVec2(shape);

		//配列わからん
		Circle enemies{ enemypos,20 };

		double directionx = 0.0;
		double directiony = 0.0;

		bool shotenable = false;
		bool enemyenable = true;

		const double speed = 200;

		const Font font{ FontMethod::MSDF,48 };

		double timeLeft = 120.0;

		//while (System::Update())
		//{
			ClearPrint();

			const double deltaTime = Scene::DeltaTime();
			//弾の位置
			shot = Circle{ shotpos,20 };
			//敵の位置
			enemy = Circle{ enemypos,20 };
			//enemies = Circle{ enemypos,20 };
			//弾を発射
			if (KeySpace.down())
			{
				shotenable = true;
			}

			//弾の移動入力
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

			if (shotenable == true)
			{
				//弾の移動
				shotpos.x += (speed * deltaTime * directionx);
				shotpos.y += (speed * deltaTime * directiony);

				//弾を無効にする
				if (shotpos.x < 0 || shotpos.x > 800 || shotpos.y < 0 || shotpos.y > 600)
				{
					shotenable = false;
					shotpos = { 400, 300 };
				}
			}
			//当たり判定
			if (enemyenable == true)
			{
				if (shot.intersects(enemy))
				{
					shotenable = false;
					shotpos = { 400, 300 };
					enemypos = RandomVec2(shape);
					directionx = 0;
					directiony = 0;
				}
			}
			//円の描画
			shot.draw(Palette::White);
			enemy.draw(Palette::Red);
			


			timeLeft -= Scene::DeltaTime();

			if (0.0 < timeLeft)
			{
				font(U"残り時間 {:.2f}"_fmt(timeLeft)).draw(30, 10, 10, Palette::White);
			}
			else
			{
				font(U"GAME CLEAR").draw(100, 85, 200, Palette::Gold);
			}
			//デバッグ
			//Print << U"{:.4f}"_fmt(shotpos);
			//Print << U"{:.4f}"_fmt(directionx);
			//Print << U"{:.4f}"_fmt(directiony);
			//Print << shotenable;

			
		//}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		Scene::SetBackground(Palette::Midnightblue);
	}
};

// タイトルシーン
class Clear : public App::Scene
{
public:

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
		Scene::SetBackground(Palette::Midnightblue);

		m_clearButton.draw(ColorF{ 1.0, m_clearTransition.value() }).drawFrame(2);

		FontAsset(U"Menu")(U"BACK TITLE").drawAt(m_clearButton.center(), ColorF{ 0.25 });
	}

private:
	Rect m_clearButton{ Arg::center = Scene::Center().movedBy(0, 200), 300, 60 };
	Transition m_clearTransition{ 0.4s, 0.2s };

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
