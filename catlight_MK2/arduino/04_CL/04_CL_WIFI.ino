
//uint32_t getParam(String name){
//  //read parameter from server, for customhmtl input
//  String value;
//  //String(value).c_str();
//  if(wm.server->hasArg(name)) {
//    value = wm.server->arg(name);
//  }
//int rgb = (int) strtol( &value[1], NULL, 16);
//
//
//  return rgb;
//}
//
//void saveParamCallback(){
//  Serial.println("[CALLBACK] saveParamCallback fired");
//  //Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
//  uint32_t number = getParam("customfieldid");
//  int r = number >> 16;
//  int g = number >> 8 & 0xFF;
//  int b = number & 0xFF;
//  for (int i=0; i<LED_COUNT; i++){
//    strip.setBrightness(200);
//    strip.setPixelColor(i, strip.Color(r,g,b));
//    strip.show();
//  }
//}

void STARTWIFI(){
  WiFi.mode(WIFI_AP_STA); // explicitly set mode, esp defaults to STA+AP  
  // wm.resetSettings();
  wm.setHostname("CATLight");
  wm.setEnableConfigPortal(true);
  wm.setConfigPortalBlocking(false);
  // set dark theme
  wm.setClass("invert");

  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
  // wm.setShowStaticFields(true); // force show static ip fields
  // wm.setShowDnsFields(true);    // force show dns field always

  //wm.setConnectTimeout(20); // how long to try to connect for before continuing
  //wm.setConfigPortalTimeout(300); // auto close configportal after n seconds
  wm.setAPClientCheck(true); // avoid timeout if client connected to softap

  // wifi scan settings
  // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
  // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
  // wm.setShowInfoErase(false);      // do not show erase button on info page
   wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons

  // add a custom input field
  //int customFieldLength = 40;


  // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\"");
  
  // test custom html input type(checkbox)
  // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\" type=\"checkbox\""); // custom html type
  
  // test custom html(radio)
  //const char* custom_radio_str  = "<head><style>body {overflow:hidden;background:black;}</style></head><body><canvas id='wheelCanvas' onclick='getColor(event)'></canvas><script type='text/javascript'>var canvas = document.getElementById('wheelCanvas');var context = canvas.getContext('2d');var imageObj = new Image();imageObj.crossOrigin = 'anonymous';var dimension;if(window.innerHeight<window.innerWidth){dimension=window.innerHeight;} else {dimension=window.innerWidth;}imageObj.onload = function() {context.drawImage(imageObj,0,0,600,600,0,0,0.98*dimension,0.98*dimension);};imageObj.src = 'https://upload.wikimedia.org/wikipedia/commons/thumb/6/6d/RGB_color_wheel_360.svg/600px-RGB_color_wheel_360.svg.png';canvas.height=window.innerHeight;canvas.width=window.innerWidth;function getColor(event) {var canvas = document.getElementById('wheelCanvas');var pixelData = canvas.getContext('2d').getImageData(event.offsetX, event.offsetY, 1, 1).data;var form = document.createElement('form');form.action='out';var nr = document.createElement('input');nr.name='R';nr.value=pixelData[0].toString();form.appendChild(nr);var ng = document.createElement('input');ng.name='G';ng.value=pixelData[1].toString();form.appendChild(ng);var nb = document.createElement('input');nb.name='B';nb.value=pixelData[2].toString(); form.appendChild(nb);form.style.display='none';document.body.appendChild(form);form.submit();   }</script></body>";
  //const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  
  const char* custom_rgb_str = "<script src='http://code.jquery.com/jquery.min.js'></script><script src='https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.4.6/jscolor.min.js'></script><input id='rgb' name='customfieldid' class='jscolor {onFineChange:'update(this)'}' value='ffcc00'><script>function update(picker) {document.getElementById('rgb').value = Math.round(picker.rgb[0]) + ', ' + Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);}</script>";                            

  // <- that was working
  //
  //const char* custom_rgb_str  = "<script src='http://code.jquery.com/jquery.min.js'></script><script src='https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.4.6/jscolor.min.js'></script><input id='rgb' name='customfieldid' class='jscolor {onFineChange:'update(this)'}' value='ffcc00'><script>function update(picker) {document.getElementById('rgb').value = Math.round(picker.rgb[0]) + ', ' + Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);}</script>";
  //new (&custom_field) WiFiManagerParameter(custom_rgb_str); // custom html input
  //wm.addParameter(&custom_field);
  //wm.setSaveParamsCallback(saveParamCallback);
  
  // custom menu via array or vector
  // 
  // menu tokens, "wifi","wifinoscan","info","param","close","sep","erase","restart","exit" (sep is seperator) (if param is in menu, params will not show up in wifi page!)
  // const char* menu[] = {"wifi","info","param","sep","restart","exit"}; 
  // wm.setMenu(menu,6);
  //std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  //wm.setMenu(menu);
  // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

  wm.autoConnect("CATLight", "catlight");
  wm.startConfigPortal("CATLight", "catlight");
  wm.startWebPortal();
}
