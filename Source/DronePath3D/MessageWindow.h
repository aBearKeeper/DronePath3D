#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MessageWindow.generated.h"

/**
 * �Զ��������Ϣ������
 */
UCLASS()
class DRONEPATH3D_API UMessageWindow : public UUserWidget
{
    GENERATED_BODY()

public:
    // ��ʼ������
    virtual void NativeConstruct() override;

    // ȷ�ϰ�ť����¼�
    UFUNCTION()
    void OnConfirmButtonClicked();

    // ���ô�����Ϣ
    UFUNCTION(BlueprintCallable, Category = "MessageWindow")
    void SetMessage(const FString& Message);

    // �󶨿ؼ�
    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;  // ȷ�ϰ�ť

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MessageText;  // ��ʾ��Ϣ��TextBlock

private:
    // �洢��Ϣ
    FString Message;
};
