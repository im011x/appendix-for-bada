var map; 
var arrMarkers = [];
var infoWindow;

var paramLat = 37.5665;
var paramLng = 126.9780;
var paramZoom = 14;
var paramImage = 'myLoc.png';
var paramInfo;

function initialize() { 

	parse_param();  // 여기서 paramLat, paramLng 값을 얻는다~ 
    
	var centerCoordinate = new google.maps.LatLng(Number(paramLat), Number(paramLng)); 
	
	var mapOptions = { 
		zoom: Number(paramZoom), 
		center: centerCoordinate, 
		mapTypeId: google.maps.MapTypeId.ROADMAP 
	}; 

	map = new google.maps.Map ( document.getElementById("map_canvas"), mapOptions ); 
	
	var marker = setMarker(Number(paramLat), Number(paramLng), paramImage);
	
	//
	infoWindow = new google.maps.InfoWindow({
		maxWidth: 150,
		width: 120,
		height: 100
	});
} 

function setMarker(latitude, longitude, icon_file) { 
	var newMarker = new google.maps.LatLng(latitude, longitude); 
	addMarker(newMarker, icon_file); 
}

function setMyLocation(latitude, longitude) {
	var marker = setMarker(latitude, longitude, paramImage);

	var currentLocation = new google.maps.LatLng ( latitude, longitude); 
	map.setCenter(currentLocation);
}

function addMarker(location, image_file) { 
	var marker = new google.maps.Marker({ 
		position: location,
		draggable : false,
		animation : google.maps.Animation.DROP,
		icon : image_file,  
		map: map
		});
	
	arrMarkers.push(marker);
	
	google.maps.event.addListener(marker, "click", showInfoWindow);
	
	/**
	google.maps.event.addListener(marker, "click", function(){
		position = marker.getPosition();
		var jsondata = {name:"MyJsBridge", data:position.toString()}; 
		var jsonText = JSON.stringify(jsondata); 
		bada.requestToNative(jsonText);
	});   
	**/
	
	return marker;
} 

function showInfoWindow(event)
{
	//alert(paramInfo);
	//infoWindow.setContent(paramInfo);

	var infoContent = 
		"위도:" + paramLat + "<br/>" + 
		"경도:" + paramLng + "<br/>" + 
		"<a href='appendix://nearStop' target='_blank'>주변정류소</a><br/>" +
		"<a href='appendix://currAddress' target='_blank'>주소 얻기</a><br/>"
		//"<a href='appendix://sendPosition' target='_blank'>위치정보 보내기</a>";
		;
	infoWindow.setContent(infoContent);

	/****
	var infoOpt = new google.maps.InfoWindowOptions({
		maxWidth: 150,
		width: 120,
		height: 100
	});
	infoWindow.setOptions(infoOpt);
	****/
	
	infoWindow.setPosition(event.latLng);
	infoWindow.open(map);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

var getUrlParameter = function ()
{   
        var ParameterObject = new Object();   
        var locate = location.href;   
  
        if(locate.indexOf("?")==-1){   
            return ParameterObject;   
        }   
  
        var parameter = locate.split("?")[1];   
        var paramAreay = parameter.split("&");   
        for ( var i=0; i<paramAreay.length; i++ )   
        {   
            var tem = paramAreay[i].split("=");   
            ParameterObject[tem[0]] = tem[1];   
        }   
        return ParameterObject;   
}   

function parse_param() 
{
    for (var ele in getUrlParameter()) {
		if (ele == 'lat') {
			paramLat = getUrlParameter()[ele]; 
		}
		else if (ele == 'lng') {
			paramLng = getUrlParameter()[ele]; 
		}
		else if (ele == 'info') {
			paramInfo = getUrlParameter()[ele]; 
		}
		else if (ele == 'zoom') {
			paramZoom = getUrlParameter()[ele]; 
		}
		else if (ele == 'markImage') {
			paramImage = getUrlParameter()[ele]; 
		}
		 
	}
}
