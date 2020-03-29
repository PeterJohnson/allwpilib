"use strict";
var connection = null;

var WebSocket = WebSocket || MozWebSocket;

// Implement bootstrap 3 style button loading support
(function($) {
  $.fn.button = function(action) {
    if (action === 'loading' && this.data('loading-text')) {
      this.data('original-text', this.html()).html(this.data('loading-text')).prop('disabled', true);
      feather.replace();
    }
    if (action === 'reset' && this.data('original-text')) {
      this.html(this.data('original-text')).prop('disabled', false);
      feather.replace();
    }
  };
}(jQuery));

// HTML escaping
var entityMap = {
  '&': '&amp;',
  '<': '&lt;',
  '>': '&gt;',
  '"': '&quot;',
  "'": '&#x27;',
  '/': '&#x2F;',
  '`': '&#x60;',
  '=': '&#x3D;'
};

function escapeHtml(string) {
  return String(string).replace(/[&<>"'`=\/]/g, function (s) {
    return entityMap[s];
  });
}

function displayStatus(message) {
  $('#status-content').html('<div id="status" class="alert alert-warning alert-dismissable fade show" role="alert"><span>' + escapeHtml(message) + '</span><button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button></div>');
}

function displaySuccess(message) {
  $('#status-content').html('<div id="status" class="alert alert-success alert-dismissable fade show" role="alert"><span>' + escapeHtml(message) + '</span><button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button></div>');
}

function dismissStatus() {
  $('.alert').alert('close');
}

// Enable and disable buttons based on connection status
var connectedButtonIds = ['addCameraButton', 'addCameraConnectedPathsList'];
var connectedButtonClasses = ['cameraName', 'cameraPath', 'cameraAlternatePaths', 'cameraPixelFormat', 'cameraWidth', 'cameraHeight', 'cameraFps', 'cameraBrightness', 'cameraWhiteBalance', 'cameraExposure', 'cameraProperties', 'streamWidth', 'streamHeight', 'streamFps', 'streamCompression', 'streamDefaultCompression', 'cameraRemove', 'cameraCopyConfig', 'cameraKey']

function displayDisconnected() {
  $('#connectionBadge').removeClass('badge-primary').addClass('badge-secondary').text('Disconnected');
  $('#visionServiceStatus').removeClass('badge-primary').removeClass('badge-secondary').addClass('badge-dark').text('Unknown Status');
  $('.cameraConnectionBadge').removeClass('badge-primary').removeClass('badge-secondary').addClass('badge-dark').text('Unknown Status');
  for (var i = 0; i < connectedButtonIds.length; i++) {
    $('#' + connectedButtonIds[i]).prop('disabled', true);
  }
  for (var i = 0; i < connectedButtonClasses.length; i++) {
    $('.' + connectedButtonClasses[i]).prop('disabled', true);
  }
  updateStreamListView([]);
}

function displayConnected() {
  $('#connectionBadge').removeClass('badge-secondary').addClass('badge-primary').text('Connected');
  for (var i = 0; i < connectedButtonIds.length; i++) {
    $('#' + connectedButtonIds[i]).prop('disabled', false);
  }
  for (var i = 0; i < connectedButtonClasses.length; i++) {
    $('.' + connectedButtonClasses[i]).prop('disabled', false);
  }
}

// Vision settings
var visionSettingsServer = {'cameras': [], 'switched cameras': []};
var visionSettingsDisplay = {'cameras': [], 'switched cameras': []};
var usbCameraList = [];

// WebSocket automatic reconnection timer
var reconnectTimerId = 0;

// Establish WebSocket connection
function connect() {
  if (connection && connection.readyState !== WebSocket.CLOSED) return;
  var serverUrl = "ws://" + window.location.hostname;
  if (window.location.port !== '') {
    serverUrl += ':' + window.location.port;
  }
  connection = new WebSocket(serverUrl, 'frcvision');
  connection.onopen = function(evt) {
    if (reconnectTimerId) {
      window.clearInterval(reconnectTimerId);
      reconnectTimerId = 0;
    }
    displayConnected();
    var msg = {
      type: 'getSourceList',
    };
    connection.send(JSON.stringify(msg));
    var msg = {
      type: 'getServerStreamsPeriodic',
      period: 1.0
    };
    connection.send(JSON.stringify(msg));
  };
  connection.onclose = function(evt) {
    displayDisconnected();
    if (!reconnectTimerId) {
      reconnectTimerId = setInterval(function() { connect(); }, 2000);
    }
  };
  // WebSocket incoming message handling
  connection.onmessage = function(evt) {
    var msg = JSON.parse(evt.data);
    if (msg === null) {
      return;
    }
    switch (msg.type) {
      case 'sourceList':
        break;
      case 'serverStreams':
        updateStreamListView(msg.streams);
        break;
      case 'usbCameraList':
        break;
      case 'systemStatus':
        for (var i = 0; i < systemStatusIds.length; i++) {
          $('#' + systemStatusIds[i]).text(msg[systemStatusIds[i]]);
        }
        break;
      case 'visionStatus':
        var elem = $('#visionServiceStatus');
        if (msg.visionServiceStatus) {
          elem.text(msg.visionServiceStatus);
        }
        if (msg.visionServiceEnabled && !elem.hasClass('badge-primary')) {
          elem.removeClass('badge-dark').removeClass('badge-secondary').addClass('badge-primary');
        } else if (!msg.visionServiceEnabled && !elem.hasClass('badge-secondary')) {
          elem.removeClass('badge-dark').removeClass('badge-primary').addClass('badge-secondary');
        }
        break;
      case 'visionSettings':
        visionSettingsServer = msg.settings;
        visionSettingsDisplay = $.extend(true, {'cameras': [], 'switched cameras': []}, visionSettingsServer);
        updateVisionSettingsView();
        break;
      case 'status':
        displayStatus(msg.message);
        break;
      case 'usbCameraList':
        usbCameraList = msg.cameras;
        updateCameraListView();
        break;
    }
  };
}

function updateStreamListView(streamList) {
  var tBody = "";
  streamList.forEach(function (stream) {
    tBody += "<tr>";
    tBody += "<td>" + escapeHtml(stream.sourceId) + "</td>";
    tBody += "<td>" + stream.remoteIp + "</td>";
    tBody += "<td>" + stream.remotePort + "</td>";
    tBody += "<td>" + stream.actualFps + "</td>";
    tBody += "<td>" + (stream.actualDataRate * 8.0e-6).toFixed(1) + "</td>";
    tBody += "</tr>";
  });
  $('#server-status-streams').html(tBody);
}

function getCameraPropertyValue(data, name) {
  if (data === null) {
    return null;
  }
  for (var i = 0; i < data.length; i++) {
    if (data[i].name === name) {
      return data[i].value;
    }
  }
  return null;
}

function updateVisionCameraView(camera, value) {
  if ('name' in value) {
    camera.find('.cameraTitle').text('Camera ' + value.name);
    camera.find('.cameraName').val(value.name);
  }
  if ('path' in value) {
    camera.find('.cameraPath').val(value.path);
  }
  camera.find('.cameraPixelFormat').val(value['pixel format']);
  camera.find('.cameraWidth').val(value.width);
  camera.find('.cameraHeight').val(value.height);
  camera.find('.cameraFps').val(value.fps);
  camera.find('.cameraBrightness').val(value.brightness);
  camera.find('.cameraWhiteBalance').val(value['white balance']);
  camera.find('.cameraExposure').val(value.exposure);
  camera.find('.cameraProperties').val(JSON.stringify(value.properties));
  if ('stream' in value && 'properties' in value.stream) {
    camera.find('.streamWidth').val(getCameraPropertyValue(value.stream.properties, 'width'));
    camera.find('.streamHeight').val(getCameraPropertyValue(value.stream.properties, 'height'));
    camera.find('.streamFps').val(getCameraPropertyValue(value.stream.properties, 'fps'));
    camera.find('.streamCompression').val(getCameraPropertyValue(value.stream.properties, 'compression'));
    camera.find('.streamDefaultCompression').val(getCameraPropertyValue(value.stream.properties, 'default_compression'));
  } else {
    camera.find('.streamWidth').val('');
    camera.find('.streamHeight').val('');
    camera.find('.streamFps').val('');
    camera.find('.streamCompression').val('');
    camera.find('.streamDefaultCompression').val('');
  }
}

function updateVisionCameraDataFromJson(i, data) {
  if (!('name' in data)) {
    data.name = visionSettingsDisplay.cameras[i].name;
  }
  if (!('path' in data)) {
    data.path = visionSettingsDisplay.cameras[i].path;
  }
  if ('properties' in data) {
    var newProps = [];
    var wbAuto = false;
    var exAuto = false;

    for (var j = 0; j < data.properties.length; j++) {
      var name = data.properties[j].name;

      // remove all raw properties
      if (name.startsWith('raw_')) {
        continue;
      }

      // brightness
      if (name === 'brightness') {
        data.brightness = data.properties[j].value;
        continue;
      }

      // white balance
      if (name === 'white_balance_temperature_auto') {
        if (data.properties[j].value === true) {
          data['white balance'] = 'auto';
          wbAuto = true;
        }
        continue;
      }
      if (name === 'white_balance_temperature') {
        if (wbAuto === false) {
          data['white balance'] = data.properties.white_balance_temperature;
        }
        continue;
      }

      // exposure
      if (name === 'exposure_auto') {
        if (data.properties[j].value === 3) {
          data.exposure = 'auto';
          exAuto = true;
        }
        continue;
      }
      if (name === 'exposure_absolute') {
        if (exAuto === false) {
          data.exposure = data.properties.exposure_absolute;
        }
        continue;
      }
      newProps.push(data.properties[j]);
    }
    data.properties = newProps;
  }
  visionSettingsDisplay.cameras[i] = data;
}

function appendNewVisionCameraView(value, i) {
  var camera = $('#cameraNEW').clone();
  camera.attr('id', 'camera' + i);
  camera.addClass('cameraSetting');
  camera.removeAttr('style');

  updateVisionCameraView(camera, value);
  camera.find('.cameraStream').attr('href', 'http://' + window.location.hostname + ':' + (1181 + i) + '/');
  camera.find('.cameraRemove').click(function() {
    visionSettingsDisplay.cameras.splice(i, 1);
    camera.remove();
    updateCameraListView();
  });
  camera.find('.cameraSettingsFile').change(function() {
    if (this.files.length <= 0) {
      return false;
    }
    var fr = new FileReader();
    fr.onload = function(e) {
      var result = JSON.parse(e.target.result);
      updateVisionCameraDataFromJson(i, result);
      updateVisionCameraView(camera, result);
    };
    fr.readAsText(this.files.item(0));
  });
  camera.find('.cameraCopyConfig').click(function() {
    fetch('http://' + window.location.hostname + ':' + (1181 + i) + '/config.json')
    .then(response => response.json())
    .then(function(result) {
      updateVisionCameraDataFromJson(i, result);
      updateVisionCameraView(camera, result);
    })
    .catch(function(error) {
      displayStatus('error reading camera config: ' + error);
    });
  });

  camera.find('[id]').each(function() {
    $(this).attr('id', $(this).attr('id').replace('NEW', i));
  });
  camera.find('[for]').each(function() {
    $(this).attr('for', $(this).attr('for').replace('NEW', i));
  });
  camera.find('[data-target]').each(function() {
    $(this).attr('data-target', $(this).attr('data-target').replace('NEW', i));
  });
  camera.find('[aria-labelledby]').each(function() {
    $(this).attr('aria-labelledby', $(this).attr('aria-labelledby').replace('NEW', i));
  });

  $('#cameras').append(camera);
}

function updateVisionSwitchedCameraView(camera, value) {
  if ('name' in value) {
    camera.find('.cameraTitle').text('Switched Camera ' + value.name);
    camera.find('.cameraName').val(value.name);
  }
  if ('key' in value) {
    camera.find('.cameraKey').val(value.key);
  }
}

function appendNewVisionSwitchedCameraView(value, i) {
  var camera = $('#switchedCameraNEW').clone();
  camera.attr('id', 'switchedCamera' + i);
  camera.addClass('cameraSetting');
  camera.removeAttr('style');

  updateVisionSwitchedCameraView(camera, value);
  camera.find('.cameraStream').attr('href', 'http://' + window.location.hostname + ':' + (1181 + visionSettingsDisplay.cameras.length + i) + '/');
  camera.find('.cameraRemove').click(function() {
    visionSettingsDisplay['switched cameras'].splice(i, 1);
    camera.remove();
  });

  camera.find('[id]').each(function() {
    $(this).attr('id', $(this).attr('id').replace('NEW', i));
  });
  camera.find('[for]').each(function() {
    $(this).attr('for', $(this).attr('for').replace('NEW', i));
  });
  camera.find('[data-target]').each(function() {
    $(this).attr('data-target', $(this).attr('data-target').replace('NEW', i));
  });
  camera.find('[aria-labelledby]').each(function() {
    $(this).attr('aria-labelledby', $(this).attr('aria-labelledby').replace('NEW', i));
  });

  $('#switchedCameras').append(camera);
}

function updateVisionSettingsView() {
  var isClient = !visionSettingsDisplay.ntmode || visionSettingsDisplay.ntmode === 'client';
  $('#visionClient').prop('checked', isClient);
  if (isClient) {
    $('#visionClientDetails').collapse('show');
  } else {
    $('#visionClientDetails').collapse('hide');
  }
  $('#visionTeam').val(visionSettingsDisplay.team);

  $('.cameraSetting').remove();
  visionSettingsDisplay.cameras.forEach(function (value, i) {
    appendNewVisionCameraView(value, i);
  });
  visionSettingsDisplay['switched cameras'].forEach(function (value, i) {
    appendNewVisionSwitchedCameraView(value, i);
  });
  updateCameraListView();
  feather.replace();
}

$('#visionSave').click(function() {
  // update json from view
  visionSettingsDisplay.ntmode = $('#visionClient').prop('checked') ? 'client' : 'server';
  visionSettingsDisplay.team = parseInt($('#visionTeam').val(), 10);
  visionSettingsDisplay.cameras.forEach(function (value, i) {
    var camera = $('#camera' + i);
    value.name = camera.find('.cameraName').val();
    value.path = camera.find('.cameraPath').val();
    value['pixel format'] = camera.find('.cameraPixelFormat').val();
    value.width = parseInt(camera.find('.cameraWidth').val(), 10);
    if (isNaN(value.width)) {
      delete value["width"];
    }
    value.height = parseInt(camera.find('.cameraHeight').val(), 10);
    if (isNaN(value.height)) {
      delete value["height"];
    }
    value.fps = parseInt(camera.find('.cameraFps').val(), 10);
    if (isNaN(value.fps)) {
      delete value["fps"];
    }

    var brightness = camera.find('.cameraBrightness').val();
    if (brightness !== '') {
      value.brightness = parseInt(brightness);
      if (isNaN(value.brightness)) {
        value.brightness = brightness;
      }
    } else {
      delete value['brightness'];
    }

    var whiteBalance = camera.find('.cameraWhiteBalance').val();
    if (whiteBalance !== '') {
      value['white balance'] = parseInt(whiteBalance);
      if (isNaN(value['white balance'])) {
        value['white balance'] = whiteBalance;
      }
    } else {
      delete value['white balance'];
    }

    var exposure = camera.find('.cameraExposure').val();
    if (exposure !== '') {
      value.exposure = parseInt(exposure);
      if (isNaN(value.exposure)) {
        value.exposure = exposure;
      }
    } else {
      delete value['exposure'];
    }

    try {
      value.properties = JSON.parse(camera.find('.cameraProperties').val());
    } catch (err) {
      delete value['properties'];
    }

    value.stream = {'properties': []};

    var streamWidth = parseInt(camera.find('.streamWidth').val(), 10);
    if (!isNaN(streamWidth)) {
      value.stream.properties.push({'name': 'width', 'value': streamWidth});
    }

    var streamHeight = parseInt(camera.find('.streamHeight').val(), 10);
    if (!isNaN(streamHeight)) {
      value.stream.properties.push({'name': 'height', 'value': streamHeight});
    }

    var streamFps = parseInt(camera.find('.streamFps').val(), 10);
    if (!isNaN(streamFps)) {
      value.stream.properties.push({'name': 'fps', 'value': streamFps});
    }

    var streamCompression = parseInt(camera.find('.streamCompression').val(), 10);
    if (!isNaN(streamCompression)) {
      value.stream.properties.push({'name': 'compression', 'value': streamCompression});
    }

    var streamDefaultCompression = parseInt(camera.find('.streamDefaultCompression').val(), 10);
    if (!isNaN(streamDefaultCompression)) {
      value.stream.properties.push({'name': 'default_compression', 'value': streamDefaultCompression});
    }
  });
  visionSettingsDisplay['switched cameras'].forEach(function (value, i) {
    var camera = $('#switchedCamera' + i);
    value.name = camera.find('.cameraName').val();
    value.key = camera.find('.cameraKey').val();
  });
  var msg = {
    type: 'visionSave',
    settings: visionSettingsDisplay
  };
  connection.send(JSON.stringify(msg));
});

$('#visionDiscard').click(function() {
  visionSettingsDisplay = $.extend(true, {}, visionSettingsServer);
  updateVisionSettingsView();
});

$('#addUsbCamera').click(function() {
  var i = visionSettingsDisplay.cameras.length;
  visionSettingsDisplay.cameras.push({});
  appendNewVisionCameraView({}, i);
  updateCameraListView();
  $('#cameraBody' + i).collapse('show');
});

$('#addSwitchedCamera').click(function() {
  var i = visionSettingsDisplay['switched cameras'].length;
  visionSettingsDisplay['switched cameras'].push({});
  appendNewVisionSwitchedCameraView({}, i);
  $('#switchedCameraBody' + i).collapse('show');
});

function updateCameraListView() {
  var addConnectedDropdown = $('#addCameraConnectedPathsList');
  addConnectedDropdown.html('');

  // disable all the alternate paths by default
  visionSettingsDisplay.cameras.forEach(function (value, k) {
    var cameraElem = $('#camera' + k);
    cameraElem.find('.cameraConnectionBadge').removeClass('badge-dark').removeClass('badge-primary').addClass('badge-secondary').text('Disconnected');
    cameraElem.find('.cameraAlternatePathsList').html('');
    cameraElem.find('.cameraAlternatePaths').prop('disabled', true);
  });

  usbCameraList.forEach(function (camera, i) {
    // See if one of the paths is an already existing camera
    // Include the "main path" as the first path
    var matchedCamera = false;
    var paths = [camera.path];
    camera.otherPaths.forEach(function (path, j) {
      paths.push(path);
    });
    paths.forEach(function (path, j) {
      visionSettingsDisplay.cameras.forEach(function (value, k) {
        var cameraElem = $('#camera' + k);
        var pathElem = cameraElem.find('.cameraPath');
        if (path === pathElem.val()) {
          matchedCamera = true;

          // show camera as connected
          cameraElem.find('.cameraConnectionBadge').removeClass('badge-dark').removeClass('badge-secondary').addClass('badge-primary').text('Connected');

          // build alternate path list
          var setAlternateDropdown = cameraElem.find('.cameraAlternatePathsList');
          setAlternateDropdown.html('');
          paths.forEach(function (altPath, j) {
            setAlternateDropdown.append('<button class="dropdown-item cameraSetAlternatePath" type="button">' + altPath + '</button>');
          });

          cameraElem.find('.cameraAlternatePaths').prop('disabled', setAlternateDropdown.html() === '');

          // hook up dropdown items to set alternate path
          setAlternateDropdown.find('.cameraSetAlternatePath').click(function() {
            pathElem.val($(this).text());
          });
        }
      });
    });

    if (!matchedCamera) {
      // add it to add connected camera list
      addConnectedDropdown.append('<h5 class="dropdown-header">' + camera.name + '</h5>');
      paths.forEach(function (path, j) {
        addConnectedDropdown.append('<button class="dropdown-item addCameraConnectedPathItem" type="button">' + path + '</button>');
      });
    }
  });

  $('#addConnectedCamera').prop('disabled', addConnectedDropdown.html() === '');

  // hook up dropdown items to create cameras
  addConnectedDropdown.find('.addCameraConnectedPathItem').click(function() {
    var i = visionSettingsDisplay.cameras.length;
    var camera = {"path": $(this).text()};
    visionSettingsDisplay.cameras.push(camera);
    appendNewVisionCameraView(camera, i);
    updateCameraListView();
    $('#cameraBody' + i).collapse('show');
  });
}

// Start with display disconnected and start initial connection attempt
displayDisconnected();
updateVisionSettingsView();
connect();
