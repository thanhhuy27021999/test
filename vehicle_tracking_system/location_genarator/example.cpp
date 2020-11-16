#include "tinyxml.h"
#include <iostream>
using namespace std;

//#define READ

int main()
{

// #ifndef READ
// 	TiXmlDocument doc("Authors.xml");
// 	//Load tài liệu XML lên bộ nhớ
// 	if (!doc.LoadFile())
// 	{
// 		printf("%s", doc.ErrorDesc());
// 		return -1;
// 	}


// 	//Lấy thông tin thẻ root
// 	TiXmlElement* root = doc.RootElement();
// 	TiXmlElement* author = nullptr;
// 	int id;


// 	for (author = root->FirstChildElement(); author != NULL; author = author->NextSiblingElement())
// 	{
// 		author->QueryIntAttribute("id", &id);
// 		TiXmlElement* name = author->FirstChildElement();
// 		TiXmlElement* age = name->NextSiblingElement();
// 		cout << id << " ";
// 		cout << name->GetText() << " ";
// 		cout << age->GetText() << endl;
// 	}

// 	//int id;
// 	////Tìm phần tử con đầu tiên của node root
// 	//TiXmlElement* child1 = root->FirstChildElement();
// 	////Lấy ra id của Author
// 	//child1->QueryIntAttribute("id", &id);
// 	////Truy vấn đến phần tử con đầu tiên của child1
// 	//TiXmlElement* name = child1->FirstChildElement();
// 	////Truy vấn đến phần tử tiếp theo cùng cấp name
// 	//TiXmlElement* age = name->NextSiblingElement();

// 	////Xuất id
// 	//cout << id << " ";
// 	////Xuất name
// 	//cout << name->GetText() << " ";
// 	////Xuất age
// 	//cout << age->GetText() << endl;

// 	////Truy xuất đến phần tử tiếp theo cùng cấp với child1
// 	//TiXmlElement* child2 = child1->NextSiblingElement();
// 	////Lấy ra id của Author
// 	//child2->QueryIntAttribute("id", &id);
// 	////Truy vấn đến phần tử con đầu tiên của child2
// 	//name = child2->FirstChildElement();
// 	////Truy vấn đến phần tử tiếp theo cùng cấp name
// 	//age = name->NextSiblingElement();

// 	////Xuất thông tin của author
// 	//cout << id << " ";
// 	//cout << name->GetText() << " ";
// 	//cout << age->GetText() << endl;
// #else

	//Tạo đối tượng quản lý tài liệu XML
	TiXmlDocument doc;

	//Tạo chỉ thị của tài liệu XML bao gồm version, endcoding sau đó thêm dec vào tài liệu
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
	//Thêm dec vào tài liệu
	doc.LinkEndChild(dec);

	//Tạo comment và thêm comment vào tài liệu
	TiXmlComment *cmt = new TiXmlComment("Demo read, write, edit XML document using TinyXML library");
	doc.LinkEndChild(cmt);

	//Tạo node root và thêm root vào tài liệu
	TiXmlElement* root = new TiXmlElement("Authors");
	doc.LinkEndChild(root);


	//Tạo Author1
	TiXmlElement* author1 = new TiXmlElement("SenSor1");
	//Set id cho author1
	author1->SetAttribute("Id", 1);
	//Thêm author1 vào root
	root->LinkEndChild(author1);


	//Tạo Author2
	TiXmlElement* author2 = new TiXmlElement("SenSor2");
	//Set id cho author2
	author2->SetAttribute("id", 268);
	//Thêm author2 vào root
	root->LinkEndChild(author2);

	//Tạo node Name 
	TiXmlElement* author1_name = new TiXmlElement("Name");
	author1->LinkEndChild(author1_name);
	TiXmlText* name_content_1 = new TiXmlText("Kevin La");
	author1_name->LinkEndChild(name_content_1);

	//Tạo node Age
	TiXmlElement* author1_age = new TiXmlElement("Age");
	author1->LinkEndChild(author1_age);
	TiXmlText* age_content_1 = new TiXmlText("27");
	author1_age->LinkEndChild(age_content_1);


	//Tạo node Name 
	TiXmlElement* author2_name = new TiXmlElement("Name");
	author2->LinkEndChild(author2_name);
	TiXmlText* name_content_2 = new TiXmlText("Nguyen Van Nghia");
	author2_name->LinkEndChild(name_content_2);

	//Tạo node Age
	TiXmlElement* author2_age = new TiXmlElement("Age");
	author2->LinkEndChild(author2_age);
	TiXmlText* age_content_2 = new TiXmlText("20");
	author2_age->LinkEndChild(age_content_2);

	doc.SaveFile("Authors_Write.xml");

//#endif

	return 0;
}