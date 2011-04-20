#define OK_PRESSED 'USOK'

class OKButton  : public BButton
{
	public:
		OKButton(BRect frame);
		virtual void	MessageReceived(BMessage* message);
		virtual void	KeyDown(BMessage* message);
		
};

OKButton::OKButton(BRect frame)
	:BButton(frame,"OKButton","OK",new BMessage(OK_PRESSED))
{
}

void OKButton::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
/*		case OK_PRESSED:
			Quit();
		break;*/
		case B_KEY_DOWN:
		system("alert bkeydown");
		break;
		case B_UNMAPPED_KEY_DOWN:
			system("alert bukeydown");
		break;
		default:
			BButton::MessageReceived(message);
	}
}




//////////////////////////////////////////////////////////
class SelKey : public BWindow
{
	public:
		SelKey(char *str);

		virtual void	MessageReceived(BMessage* message);

		char *ret;
};

SelKey::SelKey(char *str)
	: BWindow(BRect(100,100,200,150), "Hold down keys, then hit OK",B_MODAL_WINDOW,B_NOT_RESIZABLE)
{
	OKButton *okb;
	okb=new OKButton(BRect(0,0,80,20));
	AddChild(okb);
	okb->MakeFocus();
	ret=str;
}

void SelKey::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case B_KEY_DOWN:
		system("alert keydown");
		break;
		case B_UNMAPPED_KEY_DOWN:
		system("alert ukeydown");
		
		break;
		case OK_PRESSED:
			Quit();
		
		break;
		default:
			BWindow::MessageReceived(message);
	}
}


