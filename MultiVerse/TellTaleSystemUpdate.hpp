#ifndef __TELLTALESYSTEMUPDATE_HPP__
#define __TELLTALESYSTEMUPDATE_HPP__

#include "TellTaleSystem.hpp"
#include "LevelScript.hpp"

void TellTaleScene::Update(Application & App, Level * Lvl)
{
    if(Playing)
    {
        Time += App.GetFrameTime();

        if(Time >= Duration)
        Stop();

        if(Type == 'S' && FocusCamOnPlayer)
        {
            Vector2f position = PlayerCharacter->Position;
            View & view = App().GetDefaultView();
            Vector2f CamPos = Vector2f(0.f, 0.f);

            if(position.x > view.GetRect().Right - view.GetHalfSize().x + CamPos.x)
            {
                Lvl->MoveView(position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
                view.Move(position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
            }
            else
            if(position.x < view.GetRect().Right - view.GetHalfSize().x + CamPos.x)
            {
                Lvl->MoveView(position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
                view.Move(position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
            }

            if(position.y > view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y)
            {
                Lvl->MoveView(0, position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
                view.Move(0, position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
            }
            else
            if(position.y < view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y)
            {
                Lvl->MoveView(0, position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
                view.Move(0, position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
            }
        }
    }
}

#endif
