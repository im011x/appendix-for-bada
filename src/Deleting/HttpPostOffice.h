/*
 * HttpPostOffice.h
 *
 *  Created on: 2012. 6. 17.
 *      Author: benny
 */

#ifndef HTTPPOSTOFFICE_H_
#define HTTPPOSTOFFICE_H_

#include "OpenApiBase.h"
#include "HandleHttp.h"

class HttpPostOffice
	: public OpenApiBase
{
public:
	HttpPostOffice(IHttpFirstResponder * pListener);
	virtual ~HttpPostOffice();

public:
	void 		GetCodeByAddress(const String & keywords);
	void 		ParseCodeByAddress(ByteBuffer* pBuffer);


};

/*
 *
지번조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getLotNumberList?
&brtcCd=서울
&signguCd=양천구
&emdCd=신정동

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getLotNumberList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울&signguCd=양천구&emdCd=신정동


읍면동 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getEupMyunDongList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울&signguCd=양천구
&brtcCd=서울
&signguCd=양천구

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getEupMyunDongList?


시군구 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?
&brtcCd=서울

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?brtcCd=대구&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울


광역시/도 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getBorodCityList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D


복합검색 - 원하는 결과 얻기 힘드네 ...
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getComplexList?
&areaNm=ex)지역명,건물명
&searchSe= and/or
&srchwrd=ex) 지역명,건물명

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getComplexList?areaNm=구로&searchSe=and&srchwrd=신도림&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D


상세검색
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?
&searchSe=dong/post/sido
&srchwrd=ex)공평동

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=신도림&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=post&srchwrd=133120&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=sido&srchwrd=경주시&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D

- dong : 동/읍/면 이름, 아파트/건물명
- post : 우편번호
- sido : 시/군/구

----------------- try --------------



지번조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getLotNumberList?
&brtcCd=서울
&signguCd=양천구
&emdCd=신정동

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getLotNumberList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울&signguCd=양천구&emdCd=신정동


읍면동 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getEupMyunDongList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울&signguCd=양천구
&brtcCd=서울
&signguCd=양천구

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getEupMyunDongList?



시군구 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?
&brtcCd=서울

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?brtcCd=대구&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getSiGunGuList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D&brtcCd=서울



광역시/도 주소조회
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getBorodCityList?serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D





복합검색 - 원하는 결과 얻기 힘드네 ...
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getComplexList?
&areaNm=ex)지역명,건물명
&searchSe= and/or
&srchwrd=ex) 지역명,건물명

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getComplexList?areaNm=구로&searchSe=and&srchwrd=신도림&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D





상세검색
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?
&searchSe=dong/post/sido
&srchwrd=ex)공평동

http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=신도림&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=post&srchwrd=133120&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=sido&srchwrd=경주시&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D

- dong : 동/읍/면 이름, 아파트/건물명
- post : 우편번호
- sido : 시/군/구


http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=성수동2가 325&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=post&srchwrd=133120&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=dong&srchwrd=공평동&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D


http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=dong&srchwrd=세종로 18&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=세종로 18&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=post&srchwrd=세종로 18&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D

http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=신정동 1306&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=post&srchwrd=133-120&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D

http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=dong&srchwrd=신정동&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D




http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=dong&srchwrd=신정7동 신시가지11단지아파트 1101&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=dong&srchwrd=신정7동 1306&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=신정7동 1306&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=경기도 여주군 여주읍 세종로 17&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D



http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=세양&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=우성아파트&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=신도림&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=신정동&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=road&srchwrd=세양청마루&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=sido&srchwrd=구로구&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D
http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?searchSe=dong&srchwrd=신정7동&serviceKey=gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D






http://maps.googleapis.com/maps/api/geocode/xml?postal_code=10123&region=ko


http://maps.googleapis.com/maps/api/geocode/xml?postal_code=133120&region=ko&sensor=false
http://maps.googleapis.com/maps/api/geocode/xml?address=%EC%84%B1%EC%88%98%EB%8F%99&region=ko&sensor=false

http://maps.googleapis.com/maps/api/geocode/xml?address=%EC%84%B1%EC%88%98%EB%8F%99&region=ko&sensor=false
http://maps.googleapis.com/maps/api/geocode/xml?address=%EC%84%B1%EB%8F%99%EA%B5%AC&region=ko&sensor=false



http://maps.googleapis.com/maps/api/geocode/xml?region=ko&sensor=false&address=서울특별시 양천구 신정동 177





 */

#endif /* HTTPPOSTOFFICE_H_ */
