const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<style>
  .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 5px; background: #FFD65C;
    outline: none; -webkit-transition: .2s; transition: opacity .2s;}
  .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 12px; height: 12px; background: #003249; cursor: pointer;}
  .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
</style>
<body>
  <p>HUB75 Pixel Art display by mzashh</p>
  <p>Firmware: %FIRMWARE%</p>
  <p>Free Storage: <span id="freeflash">%FREEFLASH%</span> | Used Storage: <span id="usedflash">%USEDFLASH%</span> | Total Storage: <span id="totalflash">%TOTALFLASH%</span></p>
  <p>
  <button onclick="logoutButton()">Logout</button>
  <button onclick="rebootButton()">Reboot</button>
  <button onclick="listFilesButton()">List Files</button>
  <button onclick="showUploadButtonFancy()">Upload File</button>
  </p>
  <p>
    <label>
    <input type="checkbox" id="gifToggle" checked onchange="toggleGIF(this)"> 
    Play GIFs
    </label>
    &nbsp;&nbsp;
    <label>
    <input type="checkbox" id="loopGifToggle" checked onchange="toggleLoopGif(this)"> 
    Loop GIFs
    </label>
    &nbsp;&nbsp;
    <label>
    <input type="checkbox" id="clockToggle" checked onchange="toggleClock(this)"> 
    Show Clock 
    </label>
    &nbsp;&nbsp;
    <label for="r">Red:</label>
    <input type="number" id="r" min="0" max="255" value="255">
    <label for="g">Green:</label>
    <input type="number" id="g" min="0" max="255" value="255">
    <label for="b">Blue:</label>
    <input type="number" id="b" min="0" max="255" value="255"> (0 = Off, 255 = Brightest)
    <button onclick="setColor()">Set Color</button>
</p>
<p>
  <label>
    <input type="checkbox" id="scrollTextToggle" onchange="toggleScrollText(this)"> 
    Scrolling Text
  </label>
  &nbsp;&nbsp;
  <label for="scrollText">Enter Text:</label>
  <input type="text" id="scrollText" placeholder="Enter your text here">
  &nbsp;&nbsp;
    <label for="fontSizeToggle">Font Size:</label>
  <select id="fontSizeToggle">
    <option value="1">Small</option>
    <option value="2" selected>Normal</option>
    <option value="3">Big</option>
    <option value="4">Huge</option>
  </select>
  &nbsp;&nbsp;
  <label for="scrollSpeed">Scroll Speed:</label>
  <input type="number" id="scrollSpeed" min="1" max="150" value="50"> (0 = Fast, 150 = Slowest)
  <button onclick="sendScrollTextData()">Send</button>
</p>
  <p>Brightness: <span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
  <p id="status"></p>
  <p id="detailsheader"></p>
  <p id="details"></p>
  <div id="fileList"></div>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
function setColor() {
    const r = document.getElementById("r").value;
    const g = document.getElementById("g").value;
    const b = document.getElementById("b").value;

    fetch(`/setColor?r=${r}&g=${g}&b=${b}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert("Color updated successfully!");
        })
        .catch(error => {
            console.error("Error:", error);
            alert("Failed to update color.");
        });
}
function toggleGIF(checkbox) {
    const state = checkbox.checked ? "on" : "off";
    fetch(`/toggleGIF?state=${state}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert("GIF playback state updated successfully!");
        })
        .catch(error => {
            console.error("Error:", error);
            alert("Failed to update GIF playback state.");
        });
}
function toggleLoopGif(checkbox) {
    const state = checkbox.checked ? "on" : "off";
    fetch(`/toggleLoopGif?state=${state}`)
      .then(response => response.text())
      .then(data => {
        console.log(data);
        alert(`Loop GIF ${state === "on" ? "enabled" : "disabled"}`);
      })
      .catch(error => {
        console.error("Error toggling Loop GIF:", error);
        alert("Failed to toggle Loop GIF.");
      });
  }         
function toggleClock(checkbox) {
    const state = checkbox.checked ? "on" : "off";

    // Automatically disable scrolling text if the clock is enabled
    if (state === "on") {
        document.getElementById("scrollTextToggle").checked = false;
        toggleScrollText(document.getElementById("scrollTextToggle"));
    }

    fetch(`/toggleClock?state=${state}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert("Clock state updated successfully!");
        })
        .catch(error => {
            console.error("Error:", error);
        });
}
function toggleScrollText(checkbox) {
    const isEnabled = checkbox.checked;
    const scrollTextInput = document.getElementById("scrollText");
    const fontSizeToggle = document.getElementById("fontSizeToggle");
    const scrollSpeedInput = document.getElementById("scrollSpeed");

    // Enable or disable the text input, font size toggle, and speed input
    scrollTextInput.disabled = !isEnabled;
    fontSizeToggle.disabled = !isEnabled;
    scrollSpeedInput.disabled = !isEnabled;

    // Automatically disable the clock if scrolling text is enabled
    if (isEnabled) {
        document.getElementById("clockToggle").checked = false;
        toggleClock(document.getElementById("clockToggle"));
    }

    // Send the scrolling text state to the firmware
    fetch(`/toggleScrollText?state=${isEnabled ? "on" : "off"}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert(`Scrolling text ${isEnabled ? "enabled" : "disabled"}`);
        })
        .catch(error => {
            console.error("Error toggling scrolling text:", error);
            alert("Failed to toggle scrolling text.");
        });
}

function sendScrollTextData() {
    const text = document.getElementById("scrollText").value;
    const fontSize = document.getElementById("fontSizeToggle").value;
    const speed = document.getElementById("scrollSpeed").value;

    // Send the text, font size, and speed to the firmware
    fetch(`/updateScrollText?text=${encodeURIComponent(text)}&fontSize=${fontSize}&speed=${speed}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert("Scrolling text updated successfully!");
        })
        .catch(error => {
            console.error("Error updating scrolling text:", error);
            alert("Failed to update scrolling text.");
        });
}                    
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
function rebootButton() {
  document.getElementById("fileList").innerHTML = "Invoking Reboot ...";
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/reboot", false);
  xhr.send();
  window.open("/reboot","_self");
}
function listFilesButton() {
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("fileList").innerHTML = xmlhttp.responseText;
}
function downloadDeleteButton(filename, action) {
    console.log(`downloadDeleteButton called with filename: ${filename}, action: ${action}`);

    if (action === "delete") {
        const url = `/file?name=${filename}&action=delete`;
        fetch(url)
            .then(response => response.text())
            .then(data => {
                console.log(data);
                alert("File deleted successfully!");
                location.reload(); // Reload the page to update the file list
            })
            .catch(error => {
                console.error("Error deleting file:", error);
                alert("Failed to delete file.");
            });
    } else if (action === "download") {
        const url = `/file?name=${filename}&action=download`;
        window.open(url, "_blank"); // Open the file in a new tab for download
    } else if (action === "play") {
        const url = `/file?name=${filename}&action=play`;
        fetch(url)
            .then(response => response.text())
            .then(data => {
                console.log(data);
                alert(`Playing GIF: ${filename}`);
            })
            .catch(error => {
                console.error("Error playing GIF:", error);
                alert("Failed to play GIF.");
            });
    }
}
function showUploadButtonFancy() {
  document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
  document.getElementById("status").innerHTML = "";
  var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
  document.getElementById("fileList").innerHTML = uploadform;
  var uploadform =
  "<form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
  "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
  "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
  "<h3 id=\"status\"></h3>" +
  "<p id=\"loaded_n_total\"></p>" +
  "</form>";
  document.getElementById("fileList").innerHTML = uploadform;
}
window.currentPage = 1; // Track the current page globally

window.navigatePage = function (page) {
    console.log("Navigating to page:", page); // Debug log
    currentPage = page; // Update the current page tracker

    fetch(`/list?page=${page}`)
        .then(response => response.text())
        .then(data => {
            console.log("Received data:", data); // Debug log
            document.getElementById("fileList").innerHTML = data;

            // Debug: Verify that the buttons are correctly generated
            const buttons = document.querySelectorAll("#fileList button");
            buttons.forEach(button => {
                console.log("Button found:", button.outerHTML); // Debug log
            });
        })
        .catch(error => {
            console.error("Error fetching page:", error);
        });
};
function _(el) {
  return document.getElementById(el);
}
function uploadFile() {
  var file = _("file1").files[0];
  // alert(file.name+" | "+file.size+" | "+file.type);
  var formdata = new FormData();
  formdata.append("file1", file);
  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/");
  ajax.send(formdata);
}
function progressHandler(event) {
  //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
  _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  _("progressBar").value = Math.round(percent);
  _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
  if (percent >= 100) {
    _("status").innerHTML = "Please wait, writing file to filesystem";
  }
}
function completeHandler(event) {
  _("status").innerHTML = "Upload Complete";
  _("progressBar").value = 0;
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("status").innerHTML = "File Uploaded";
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("fileList").innerHTML = xmlhttp.responseText;
}
function errorHandler(event) {
  _("status").innerHTML = "Upload Failed";
}
function abortHandler(event) {
  _("status").innerHTML = "inUpload Aborted";
}
</script>
</body>
</html>
)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p><a href="/">Log Back In</a></p>
</body>
</html>
)rawliteral";

// reboot.html base upon https://gist.github.com/Joel-James/62d98e8cb3a1b6b05102
const char reboot_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
</head>
<body>
<h3>
  Rebooting, returning to main page in <span id="countdown">30</span> seconds
</h3>
<script type="text/javascript">
  var seconds = 5;
  function countdown() {
    seconds = seconds - 1;
    if (seconds < 0) {
      window.location = "/";
    } else {
      document.getElementById("countdown").innerHTML = seconds;
      window.setTimeout("countdown()", 1000);
    }
  }
  countdown();
</script>
</body>
</html>
)rawliteral";
