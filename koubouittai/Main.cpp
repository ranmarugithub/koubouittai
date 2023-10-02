# include <Siv3D.hpp>

void Main()
{
	Circle circle;

	double directionx = 0.0;
	double directiony = 0.0;
	bool shotenable = false;

	Vec2 pos{ 400, 300 };

	const double speed = 200;

	while (System::Update())
	{
		ClearPrint();

		const double deltaTime = Scene::DeltaTime();

		//円を描く
		Circle(pos, 20).draw(Palette::White);

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
			pos.x += (speed * deltaTime * directionx);
			pos.y += (speed * deltaTime * directiony);

			//弾を無効にする
			if (pos.x < 0 || pos.x > 800 || pos.y < 0 || pos.y > 600)
			{
				shotenable = false;
				pos = { 400, 300 };
			}
		}

		//デバッグ
		Print << U"{:.4f}"_fmt(pos);
		Print << U"{:.4f}"_fmt(directionx);
		Print << U"{:.4f}"_fmt(directiony);
		Print << shotenable;
	}
}
