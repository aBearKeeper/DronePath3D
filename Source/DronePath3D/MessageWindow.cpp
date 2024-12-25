#include "MessageWindow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMessageWindow::NativeConstruct()
{
    Super::NativeConstruct();

    // �󶨰�ť����¼�
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UMessageWindow::OnConfirmButtonClicked);
    }
}

void UMessageWindow::OnConfirmButtonClicked()
{
    // ���ȷ�Ϻ��Ƴ�����
    RemoveFromParent();
}

void UMessageWindow::SetMessage(const FString& NewMessage)
{
    Message = NewMessage;

    // ����TextBlock��ʾ������Ϣ
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(Message));
    }
}
