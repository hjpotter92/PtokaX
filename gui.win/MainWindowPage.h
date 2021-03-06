/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2004-2015  Petr Kozelka, PPK at PtokaX dot org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//------------------------------------------------------------------------------
#ifndef MainWindowPageH
#define MainWindowPageH
//------------------------------------------------------------------------------

class MainWindowPage {
public:
    HWND m_hWnd;

    MainWindowPage();
    virtual ~MainWindowPage() { };

    static LRESULT CALLBACK StaticMainWindowPageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual bool CreateMainWindowPage(HWND hOwner) = NULL;
    virtual void UpdateLanguage() = NULL;
    virtual char * GetPageName() = NULL;
    virtual void FocusFirstItem() = NULL;
    virtual void FocusLastItem() = NULL;
protected:
    void CreateHWND(HWND hOwner);
private:
    MainWindowPage(const MainWindowPage&);
    const MainWindowPage& operator=(const MainWindowPage&);

    virtual LRESULT MainWindowPageProc(UINT uMsg, WPARAM wParam, LPARAM lParam) = NULL;
};
//------------------------------------------------------------------------------

LRESULT CALLBACK FirstButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LastButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//------------------------------------------------------------------------------

#endif
