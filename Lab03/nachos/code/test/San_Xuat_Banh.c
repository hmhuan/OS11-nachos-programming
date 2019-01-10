#include "syscall.h"

void main() {
	int bot, nguyen_lieu, gia_vi, lam_banh;

	CreateSemaphore("bot",0);
	CreateSemaphore("nguyen_lieu",0);
	CreateSemaphore("gia_vi",0);

	bot = Exec("./test/Nhap_Bot");
	nguyen_lieu = Exec("./test/Nhap_Nguyen_Lieu");
	gia_vi = Exec("./test/Nhap_Gia_Vi");
	lam_banh = Exec("./test/Lam_Banh");

	Join(bot);
	Join(nguyen_lieu);
	Join(gia_vi);
	Join(lam_banh);
}
