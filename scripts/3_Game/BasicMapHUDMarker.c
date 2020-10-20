class BasicMapHUDMarker {
		
	protected string 					LayoutPath = "BasicMap/gui/layouts/Marker.layout";
	
	protected ref BasicMapMarker 		m_MarkerData;
	
	protected Widget 					layoutRoot;
	
	protected Widget					m_Marker;
	
	protected ImageWidget				m_Icon;
	protected TextWidget				m_Name;
	protected TextWidget				m_Distance;
	
	void Init(ref BasicMapMarker markerData){
		m_MarkerData 	= markerData;
		layoutRoot	 	= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(LayoutPath));
		
		m_Marker 		= Widget.Cast( layoutRoot.FindAnyWidget( "Marker" ) );
		
		m_Icon			= ImageWidget.Cast( layoutRoot.FindAnyWidget( "Icon" ) );
		m_Name			= TextWidget.Cast( layoutRoot.FindAnyWidget( "Name" ) );
		m_Distance		= TextWidget.Cast( layoutRoot.FindAnyWidget( "Distance" ) );
		if (!m_MarkerData){
			return;
		}
		
		m_Name.SetText(m_MarkerData.Name);
		if (m_MarkerData.Icon != ""){
			m_Icon.LoadImageFile(0, m_MarkerData.Icon);
		} else {
			m_Marker.Show(false);
		}
		m_Icon.SetColor(m_MarkerData.GetColour());
		m_Name.SetColor(m_MarkerData.GetColour());
		m_Distance.SetColor(m_MarkerData.GetColour());
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Update, 14, true);
	}
	
	void ~BasicMapHUDMarker(){
		m_Marker.Show(false);
		m_Icon.Show(false);
		m_Name.Show(false);
		m_Distance.Show(false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	void Update(){	
		if (m_MarkerData){
			if (MarkerVisibleOnScreen() && m_MarkerData.OnHUD){
				
				float x;
				float y;
				vector screenPosition = GetGame().GetScreenPos(m_MarkerData.GetPosition());
				float distance = Math.Round(screenPosition[2]);
				x = Math.Round(screenPosition[0]);
		        y = Math.Round(screenPosition[1]);
				string distanceText = distance.ToString() + "m";
				if (distance > 1000){
					distance = distance / 100;
					distance = Math.Round(distance);
					distance = distance / 10;
					distanceText = distance.ToString() + "km";
				}
				m_Distance.SetText(distanceText);
				m_Marker.Show(true);
				layoutRoot.Show(true);
				layoutRoot.SetPos(x, y);
			} else{
				layoutRoot.Show(false);
			}
		} else {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.Update);
			Show(false);
		}
	}
	
	void Show(bool state = true){
		m_Marker.Show(state);
	}
	
	private bool MarkerVisibleOnScreen()
    {
		if (m_MarkerData){
	        vector position = m_MarkerData.GetPosition();
	        vector screenPositionRelative = GetGame().GetScreenPosRelative(position);
	        if (screenPositionRelative[0] >= 1 || screenPositionRelative[0] == 0 || screenPositionRelative[1] >= 1 || screenPositionRelative[1] == 0)
	        {
	            return false;
	        }
	        else if (screenPositionRelative[2] < 0)
	        {
	            return false;
	        }
			return true;
		}
        return false;
    }
}