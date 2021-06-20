#pragma once

#define TFR 60.f

namespace Core
{
	namespace API
	{
		class Time
		{
		private:
			float			lastTime = 0.f;

			float			ts = 1.f;					//time scale

			float			dt = 0.f;					//delta time

			float			targetFrameRate = TFR;
			float			fdt = 1 / targetFrameRate;	//fixed delta time
			int				f = 1;						//fix count

		public:

			float&	timeScale();
			float	deltaTime();

			float	fixedDeltaTime();
			int		fixing();
			void	resetLastTime();

			void	update();
		};
	}
}