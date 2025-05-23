#include "ParserTestFixture.h"

HMODULE ParserTestFixture::hDll = nullptr;
ParserTestFixture::CreateParserFunc ParserTestFixture::CreateParser = nullptr;
ParserTestFixture::ParserCalcFunc ParserTestFixture::Calc = nullptr;
ParserTestFixture::DestroyParserFunc ParserTestFixture::DestroyParser = nullptr;