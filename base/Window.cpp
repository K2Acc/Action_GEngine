#include "Window.h"

Window::Window(const std::string &name, const int width, const int height):
	window_name(name),
	window_width(width),
	window_height(height)
{
}

Window::~Window()
{
}

void Window::Create()
{
	//�E�B���h�E�N���X�ݒ�
	//WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		//�E�B���h�E�v���V�[�W���ݒ�
	w.lpszClassName = (LPCWSTR)window_name.c_str();			//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);		//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	//�E�B���h�E�T�C�Y{ X���W Y���W ���� �c��}
	RECT wrc = {0, 0, window_width, window_height};
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	///�E�B���h�E�I�u�W�F�N�g�̐���
	//HWND hwnd = nullptr;
	hwnd = CreateWindow(
		w.lpszClassName,			//�N���X��
		L"DirectXGame",				//�^�C�g���o�[����
		WS_OVERLAPPEDWINDOW,		//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,				//�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,				//�\��Y���W(OS�ɔC����)
		wrc.right  - wrc.left,		//�E�B���h�E����
		wrc.bottom - wrc.top,		//�E�B���h�E�c��
		nullptr,					//�e�E�B���h�E�n���h��
		nullptr,					//���j���[�n���h��
		w.hInstance,				//�Ăяo���A�v���P�[�V�����n���h��
		nullptr						//�I�v�V����
	);
	//�E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);
}

void Window::Finalize()
{
	///�E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg){
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}