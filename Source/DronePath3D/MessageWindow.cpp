#include "MessageWindow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMessageWindow::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定按钮点击事件
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UMessageWindow::OnConfirmButtonClicked);
    }
}

void UMessageWindow::OnConfirmButtonClicked()
{
    // 点击确认后移除弹窗
    RemoveFromParent();
}

void UMessageWindow::SetMessage(const FString& NewMessage)
{
    Message = NewMessage;

    // 设置TextBlock显示错误信息
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(Message));
    }
}
