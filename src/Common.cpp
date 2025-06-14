#include "Common.hpp"

QString g_dialogPathCache = ".";

bool versionStringMatch(QString v1, QString v2, bool exact) {
	QStringList rev1 = v1.split(".");
	QStringList rev2 = v2.split(".");
	if (rev1.length() < 3 || rev2.length() < 3) return false;
	if (rev1[0] != rev2[0]) return false;
	if (rev1[1] != rev1[1]) return false;
	if (exact && rev1[2] != rev2[2]) return false;
	return true;
}

int versionStringCompare(QString v1, QString v2, bool exact) {
	QStringList rev1 = v1.split(".");
	QStringList rev2 = v2.split(".");
	if (rev1.length() < 3 || rev2.length() < 3) return 0;
	if (rev1[0].toInt() > rev2[0].toInt()) return 1;
	else if (rev1[0].toInt() < rev2[0].toInt()) return -1;
	else {
		if (rev1[1].toInt() > rev2[1].toInt()) return 1;
		else if (rev1[1].toInt() < rev2[1].toInt()) return -1;
		else if (exact) {
			if (rev1[2].toInt() > rev2[2].toInt()) return 1;
			else if (rev1[2].toInt() < rev2[2].toInt()) return -1;
		}
	}
	return 0;
}

char randChar() {
	return (char)(32 + (rand() % 95));
}

std::uint64_t roundUp(std::uint64_t num, std::uint64_t multiple) {
	if (multiple == 0) { return num; }
	std::uint64_t r = num % multiple;
	if (r == 0) { return num; }
	return num + multiple - r;
}

std::uint32_t crc32Calculate(const void* data, std::size_t length, std::uint32_t previousCRC) {
	std::uint32_t crc = previousCRC;
	const std::uint8_t* current = (const std::uint8_t*)data;
	while (length-- != 0) {
		crc = (crc >> 8) ^ crc32Lookup[(crc & 0xFF) ^ *current++];
	}
	return crc;
}

const std::uint32_t crc32Lookup[256] = {
	//const std::uint32_t Polynomial = 0xEDB88320;
	//for (int i = 0; i <= 0xFF; i++) {
	//  std::uint32_t crc = i;
	//  for (int j = 0; j < 8; j++)
	//    crc = (crc >> 1) ^ ((crc & 1) * Polynomial);
	//  crc32Lookup[i] = crc;
	//}
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
	0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
	0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
	0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
	0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
	0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
	0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
	0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
	0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
	0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
	0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
	0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
	0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
	0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
	0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

QString imageFormatString(QImage::Format format) {
	switch (format) {
	case QImage::Format_Mono: return "Mono (MSB)"; break;
	case QImage::Format_MonoLSB: return "Mono (LSB)"; break;
	case QImage::Format_Indexed8: return "Indexed (8-bit)"; break;
	case QImage::Format_RGB32: return "32-bit RGB"; break;
	case QImage::Format_ARGB32: return "32-bit ARGB"; break;
	case QImage::Format_ARGB32_Premultiplied: return "32-bit ARGB (Premultiplied)"; break;
	case QImage::Format_RGB16: return "16-bit RGB (5-6-5)"; break;
	case QImage::Format_ARGB8565_Premultiplied: return "24-bit ARGB (Premultiplied) (8-5-6-5)"; break;
	case QImage::Format_RGB666: return "24-bit RGB (6-6-6)"; break;
	case QImage::Format_ARGB6666_Premultiplied: return "24-bit ARGB (Premultiplied) (6-6-6-6)"; break;
	case QImage::Format_RGB555: return "16-bit RGB (5-5-5)"; break;
	case QImage::Format_ARGB8555_Premultiplied: return "24-bit ARGB (Premultiplied) (8-5-5-5)"; break;
	case QImage::Format_RGB888: return "24-bit RGB (8-8-8)"; break;
	case QImage::Format_RGB444: return "16-bit RGB (4-4-4)"; break;
	case QImage::Format_ARGB4444_Premultiplied: return "16-bit ARGB (Premultiplied) (4-4-4-4)"; break;
	case QImage::Format_RGBX8888: return "32-bit byte-ordered RGB(x) (4-4-4-x)"; break;
	case QImage::Format_RGBA8888: return "32-bit byte-ordered RGBA (4-4-4-4)"; break;
	case QImage::Format_RGBA8888_Premultiplied: return "32-bit byte-ordered RGBA (Premultiplied) (4-4-4-4)"; break;
	case QImage::Format_BGR30: return "32-bit (x)BGR (x-10-10-10)"; break;
	case QImage::Format_A2BGR30_Premultiplied: return "32-bit ABGR (Premultiplied) (2-10-10-10)"; break;
	case QImage::Format_RGB30: return "32-bit (x)RGB (x-10-10-10)"; break;
	case QImage::Format_A2RGB30_Premultiplied: return "32-bit ARGB (Premultiplied) (2-10-10-10)"; break;
	case QImage::Format_Alpha8: return "8-bit Alpha"; break;
	case QImage::Format_Grayscale8: return "8-bit Grayscale"; break;
	case QImage::Format_Grayscale16: return "16-bit Grayscale"; break;
	case QImage::Format_RGBX64: return "64-bit halfword-ordered RGB(x) (16-16-16-x)"; break;
	case QImage::Format_RGBA64: return "64-bit halfword-ordered RGBA (16-16-16-16)"; break;
	case QImage::Format_RGBA64_Premultiplied: return "64-bit halfword-ordered RGBA (Premultiplied) (16-16-16-16)"; break;
	case QImage::Format_BGR888: return "24-bit BGR"; break;
	case QImage::Format_RGBX16FPx4: return "4xFP16 RGB(x) (16-16-16-x)"; break;
	case QImage::Format_RGBA16FPx4: return "4xFP16 RGBA (16-16-16-16)"; break;
	case QImage::Format_RGBA16FPx4_Premultiplied: return "4xFP16 RGBA (Premultiplied) (16-16-16-16)"; break;
	case QImage::Format_RGBX32FPx4: return "4xFP32 RGB(x) (32-32-32-x)"; break;
	case QImage::Format_RGBA32FPx4: return "4xFP32 RGBA (32-32-32-32)"; break;
	case QImage::Format_RGBA32FPx4_Premultiplied: return "4xFP32 RGBA (Premultiplied) (32-32-32-32)"; break;
	case QImage::Format_CMYK8888: return "32-bit byte-ordered CMYK"; break;
	default: return "(Unknown Format)"; break;
	};
}

uint32_t imageFormatSDL(QImage::Format format) {
	switch (format) {
	case QImage::Format_Mono:        return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1, SDL_BITMAPORDER_1234, 0, 1, 0); break;
	case QImage::Format_MonoLSB:     return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1, SDL_BITMAPORDER_4321, 0, 1, 0); break;
	case QImage::Format_Indexed8:    return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX8, 0, 0, 8, 1); break;
	case QImage::Format_ARGB32:      return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_ARGB, SDL_PACKEDLAYOUT_8888, 32, 4); break;
	case QImage::Format_RGBX8888:    return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBX, SDL_PACKEDLAYOUT_8888, 32, 4); break;
	case QImage::Format_RGBA8888:    return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBA, SDL_PACKEDLAYOUT_8888, 32, 4); break;
	case QImage::Format_RGBX64:      return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU16, SDL_ARRAYORDER_RGBA, 0, 64, 8); break;
	case QImage::Format_RGBA64:      return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU16, SDL_ARRAYORDER_RGBA, 0, 64, 8); break;
	case QImage::Format_RGBX16FPx4:  return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYF16, SDL_ARRAYORDER_RGBA, 0, 64, 8); break;
	case QImage::Format_RGBA16FPx4:  return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYF16, SDL_ARRAYORDER_RGBA, 0, 64, 8); break;
	case QImage::Format_RGBX32FPx4:  return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYF32, SDL_ARRAYORDER_RGBA, 0, 128, 16); break;
	case QImage::Format_RGBA32FPx4:  return SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYF32, SDL_ARRAYORDER_RGBA, 0, 128, 16); break;
	default: return 0; break;
	};
}

QString textureGroupNameBase(const QString& name) {
	QRegularExpression regex("(\\w+)(_)(\\d+)$");
	auto match = regex.match(name);
	if (match.hasMatch()) {
		return match.captured(1);
	}
	else {
		return name;
	}
}

QString textureGroupNameIncrement(const QString& name) {
	QRegularExpression regex("(\\w+)(_)(\\d+)$");
	auto match = regex.match(name);
	if (match.hasMatch()) {
		QString base = match.captured(1);
		int number = match.captured(3).toInt();
		return QString("%1_%2").arg(base).arg(number);
	}
	else {
		return (name + "_1");
	}
}

void byteArrayPushInt8(QByteArray* array, std::uint8_t num) {
	array->push_back((char)num);
}

void byteArrayPushInt16(QByteArray* array, std::uint16_t num) {
	array->push_back((char)(num & 0xFF));
	array->push_back((char)((num >> 8) & 0xFF));
}

void byteArrayPushInt32(QByteArray* array, std::uint32_t num) {
	array->push_back((char)(num & 0xFF));
	array->push_back((char)((num >> 8) & 0xFF));
	array->push_back((char)((num >> 16) & 0xFF));
	array->push_back((char)((num >> 24) & 0xFF));
}

void byteArrayPushInt64(QByteArray* array, std::uint64_t num) {
	array->push_back((char)(num & 0xFF));
	array->push_back((char)((num >> 8) & 0xFF));
	array->push_back((char)((num >> 16) & 0xFF));
	array->push_back((char)((num >> 24) & 0xFF));
	array->push_back((char)((num >> 32) & 0xFF));
	array->push_back((char)((num >> 40) & 0xFF));
	array->push_back((char)((num >> 48) & 0xFF));
	array->push_back((char)((num >> 56) & 0xFF));
}

void byteArraySetInt8(QByteArray* array, qsizetype idx, std::uint8_t num) {
	(*array)[idx] = (char)(num);
}

void byteArraySetInt16(QByteArray* array, qsizetype idx, std::uint16_t num) {
	(*array)[idx] = (char)(num & 0xFF);
	(*array)[idx + 1] = (char)((num >> 8) & 0xFF);
}

void byteArraySetInt32(QByteArray* array, qsizetype idx, std::uint32_t num) {
	(*array)[idx] = (char)(num & 0xFF);
	(*array)[idx + 1] = (char)((num >> 8) & 0xFF);
	(*array)[idx + 2] = (char)((num >> 16) & 0xFF);
	(*array)[idx + 3] = (char)((num >> 24) & 0xFF);
}

void byteArraySetInt64(QByteArray* array, qsizetype idx, std::uint64_t num) {
	(*array)[idx] = (char)(num & 0xFF);
	(*array)[idx + 1] = (char)((num >> 8) & 0xFF);
	(*array)[idx + 2] = (char)((num >> 16) & 0xFF);
	(*array)[idx + 3] = (char)((num >> 24) & 0xFF);
	(*array)[idx + 4] = (char)((num >> 32) & 0xFF);
	(*array)[idx + 5] = (char)((num >> 40) & 0xFF);
	(*array)[idx + 6] = (char)((num >> 48) & 0xFF);
	(*array)[idx + 7] = (char)((num >> 56) & 0xFF);
}

void byteArrayPushStr(QByteArray* array, QString str, qsizetype maxLen) {
	str.truncate(maxLen);
	array->push_back(str.toStdString().data());
	for (qsizetype i = str.size(); i < maxLen; ++i) {
		array->push_back(char(0));
	}
}

void byteArrayPushStr(QByteArray* array, const char* str, qsizetype strLen, qsizetype maxLen) {
	for (qsizetype i = 0; i < maxLen; ++i) {
		if (i < strLen) {
			array->push_back(str[i]);
		}
		else {
			array->push_back(char(0));
		}
	}
}

void byteArraySetStr(QByteArray* array, qsizetype idx, QString str, qsizetype maxLen) {
	str.truncate(maxLen);
	std::string stdStr = str.toStdString();

	for (qsizetype i = 0; i < maxLen; ++i) {
		if (i < stdStr.size()) {
			(*array)[idx + i] = stdStr[i];
		}
		else {
			(*array)[idx + i] = char(0);
		}
	}
}

void byteArraySetStr(QByteArray* array, qsizetype idx, const char* str, qsizetype strLen, qsizetype maxLen) {
	for (qsizetype i = 0; i < maxLen; ++i) {
		if (i < strLen) {
			(*array)[idx + i] = str[i];
		}
		else {
			(*array)[idx + i] = char(0);
		}
	}
}

void byteArrayAlign(QByteArray* array, qsizetype alignTo) {
	// PKCS#7 padding
	if (array->size() % alignTo != 0) {
		qsizetype diff = alignTo - (array->size() % alignTo);
		uint64_t fill = std::min(0xFFULL, (uint64_t)diff);
		for (qsizetype i = 0; i < diff; ++i) {
			array->push_back((char)(fill));
		}
	}
}