/**
 * @file TewsFilesVersion.h
 */

#ifndef TEWSFILESVERSION_H_
#define TEWSFILESVERSION_H_

// C++
#include <string>

namespace TewsFiles
{
namespace Version
{

/** @return Возвращает название проекта */
std::string getProjectName();

/** @return Возвращает описание проекта */
std::string getProjectDescription();

/** @return Возвращает номер мажорной версии */
std::string getMajor();

/** @return Возвращает номер минорной версии */
std::string getMinor();

/** @return Возвращает номер патча */
std::string getPatch();

/** @return Возвращает номер ревизии */
std::string getRevision();

/** @return Возвращает информацию о проекте */
std::string getString();

} // namespace Version
} // namespace TewsFiles

#endif /* TEWSFILESVERSION_H_ */
