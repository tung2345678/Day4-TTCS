#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>
#define	LOW									0
#define BTN_PRESS							LOW
// Define Logic GPIO_PIN

#define GPIO_PIN_SET						1
#define GPIO_PIN_RESET						0
#define GPIO_PIN_LOW						0
#define GPIO_PIN_HIGH						1

// Define GPIO PIN

#define LED_GPIO_PORT						GPIOA
#define LED_GPIO_PIN						GPIO_Pin_5
#define LED_PIN5							5
#define LEDControl_SetClock					RCC_AHB1Periph_GPIOA

#define BUTTON_GPIO_PORT					GPIOC
#define BUTTON_GPIO_PIN						GPIO_Pin_13
#define BUTTON_PIN13						13
#define BUTTONControl_SetClock				RCC_AHB1Periph_GPIOC

void delay() {
	for (uint32_t i = 0; i < 500000; i++);
}
static void Led_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDControl_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led

	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;

	//Chon chan dieu khien led che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

}
static void Button_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock cho PORT C (PC13)
	RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}


static void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN,
		uint8_t Status) {
	// SET bit in BSRR Registers

	if (Status == GPIO_PIN_SET) {
		GPIOx->BSRRL = GPIO_PIN;
	}
	if (Status == GPIO_PIN_RESET) {
		GPIOx->BSRRH = GPIO_PIN;
	}
}

static uint8_t ButtonRead_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN) {
	uint8_t Read_Pin = 0x00;

	if ((GPIOx->IDR & GPIO_PIN) != (uint32_t) Bit_RESET) {
		Read_Pin = (uint8_t) Bit_SET;
	} else {
		Read_Pin = (uint8_t) Bit_RESET;
	}
	return Read_Pin;
}
int main(void) {

	Led_init();
	Button_init();

	uint8_t button_state = 1;        // trạng thái hiện tại nút
	uint8_t button_state_old = 1;    // trạng thái trước đó
	uint8_t led_state = 0;           // 0: tắt, 1: sáng

	while (1) {

		button_state = ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);

		// Phát hiện CẠNH NHẤN (từ 1 -> 0)
		if ((button_state == BTN_PRESS) && (button_state_old == 1)) {

			delay(); // chống dội phím

			led_state = !led_state; // đảo trạng thái LED

			if (led_state) {
				LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
			} else {
				LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
			}
		}

		button_state_old = button_state;
	}
}
