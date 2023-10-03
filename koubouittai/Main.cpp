# include <Siv3D.hpp>

//シーンの名前
enum class State
{
	Title,
	Game,
};

using App = SceneManager<String>;

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
		if (MouseL.down())
		{
			changeScene(U"Game");
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

		FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);

		Circle{ Cursor::Pos(), 50 }.draw(Palette::Orange);
	}
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
		Circle shot;
		Circle enemy;

		double directionx = 0.0;
		double directiony = 0.0;
		bool shotenable = false;
		bool enemyenable = true;

		Vec2 shotpos{ 400, 300 };
		Vec2 enemypos{ 500, 400 };

		const double speed = 200;

		while (System::Update())
		{
			ClearPrint();

			const double deltaTime = Scene::DeltaTime();
			shot = Circle{ shotpos,20 };
			enemy = Circle{ enemypos,20 };

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
					enemypos = { 300,200 };
				}
			}

			//円を描く
			shot.draw(Palette::White);
			enemy.draw(Palette::Red);

			//デバッグ
			Print << U"{:.4f}"_fmt(shotpos);
			Print << U"{:.4f}"_fmt(directionx);
			Print << U"{:.4f}"_fmt(directiony);
			Print << shotenable;
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		//test
	}
};

void Main()
{
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は "Title"）を登録
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");

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


