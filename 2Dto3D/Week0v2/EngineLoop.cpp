#include "EngineLoop.h"
#include "Renderer.h"
#include "ImGuiManager.h"

bool EngineLoop::bIsExit = false;
MSG EngineLoop::msg = {};

void EngineLoop:: ProcessInput() {
	while (PeekMessage(&(EngineLoop::msg), nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // 키보드 입력 메시지를 문자메시지로 변경
		DispatchMessage(&msg); // 메시지를 WndProc에 전달

		if (msg.message == WM_QUIT)
		{
			EngineLoop::bIsExit = true;
			break;
		}

		/* 추후 InputManager를 통한 인풋 로직 추가*/
#pragma region 사전과제 관련 코드 주석 처리
//	if (bBlackHole) {
//		if (msg.wParam == VK_LEFT)
//		{
//			locBlackHole.x -= 0.05f;
//		}
//		if (msg.wParam == VK_RIGHT)
//		{
//			locBlackHole.x += 0.05f;
//		}
//		if (msg.wParam == VK_UP)
//		{
//			locBlackHole.y += 0.05f;
//		}
//		if (msg.wParam == VK_DOWN)
//		{
//			locBlackHole.y -= 0.05f;
//		}
//		if (locBlackHole.x < leftBorder)
//		{
//			locBlackHole.x = leftBorder;
//		}
//		if (locBlackHole.x > rightBorder)
//		{
//			locBlackHole.x = rightBorder;
//		}
//		if (locBlackHole.y < topBorder)
//		{
//			locBlackHole.y = topBorder;
//		}
//		if (locBlackHole.y > bottomBorder)
//		{
//			locBlackHole.y = bottomBorder;
//		}
//	}
//}
//while (numBalls > UBall::ballCount)
//{
//	if (!HeadBall->CreateBall())
//	{
//		numBalls--;
//	}
//}
//while (numBalls < UBall::ballCount)
//{
//	if (numBalls <= 0) {
//		numBalls = 1;
//		break;
//	}
//	HeadBall->DeleteRandomBall();
//}
//UBall* Iter = HeadBall->NextBall;
//while (Iter)
//{
//	Iter->Update(elapsedTime);
//	Iter = Iter->NextBall;
//}

////블랙홀 존재시 위치 조정 
//if (bBlackHole) {
//	BlackHole->Location = locBlackHole;
//	if (bEating && UBall::ballCount > 1)
//	{
//		UBall* ptmp = HeadBall->NextBall;
//		while (ptmp)
//		{
//			float distance = (BlackHole->Location - ptmp->Location).Magnitude();
//			float radiusSum = BlackHole->Radius + ptmp->Radius;
//			if (distance <= radiusSum)
//			{
//				if(BlackHole->Radius< 0.9f)
//				BlackHole->Radius += ptmp->Mass * 0.0005;
//				ptmp->DeleteBall();
//				ptmp = HeadBall->NextBall;
//				if(!bAutoSpawn)
//					numBalls--;
//			}
//			ptmp = ptmp->NextBall;
//		}
//	}
#pragma endregion
	}
}

void EngineLoop::Update() {
	/* 추후 연산 로직 추가 */
}

void EngineLoop::Render(URenderer& renderer) {
	/* 추후 렌더링 로직 추가*/
	renderer.Prepare();
	renderer.PrepareShader();
	ImGuiManager::RenderImGui();
	/* 추후 렌더링 로직 추가*/
	renderer.SwapBuffer();
}

void EngineLoop::Run(URenderer& renderer)
{
	ProcessInput();
	Update();
	Render(renderer);

}