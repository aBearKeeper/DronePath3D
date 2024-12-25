#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MessageWindow.generated.h"

/**
 * 自定义错误消息弹窗类
 */
UCLASS()
class DRONEPATH3D_API UMessageWindow : public UUserWidget
{
    GENERATED_BODY()

public:
    // 初始化构造
    virtual void NativeConstruct() override;

    // 确认按钮点击事件
    UFUNCTION()
    void OnConfirmButtonClicked();

    // 设置错误消息
    UFUNCTION(BlueprintCallable, Category = "MessageWindow")
    void SetMessage(const FString& Message);

    // 绑定控件
    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;  // 确认按钮

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MessageText;  // 显示信息的TextBlock

private:
    // 存储信息
    FString Message;
};
