#pragma once
namespace UIInterModal
{
	const CString UIXAML_IA_BACKIMAGE
		(
		_T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#527BA8' Offset='0'/>")
		_T("      <GradientStop Color='#aac9de' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("</Border>") 
		);

	const CString
		UIXAML_IA_MODEL_DEFAULT 
		(
		_T("<Border Width='168' Height='118' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> </TextBlock>")  
		_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal Assignment </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MODEL_DEFAULT 
		(
		_T("<Border Width='168' Height='118' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#663333'>복합수단 통행배정 모형</TextBlock>")    
		_T("    <Border Width='164' Height='1' Background='#7c96b5' />")
		_T("    <Border Width='164' Height='1' Background='#9acae4' />")
		_T("    <TextBlock TextAlignment='Center' Padding='0, 20, 0, 10' FontSize='11' Foreground='#000000'>수단선택, 대중교통 통행배정,</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontSize='11' Foreground='#000000'>도로 통행배정 통합 모형</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_MODEL_SELECTED 
		(
		_T("<Border Width='168' Height='118' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='3,3,3,3'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#7c96b5' />")
		_T("    <Border Height='1' Background='#9acae4' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> </TextBlock>")  
		_T("    <TextBlock Padding='5, 10, 5, 1' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal Assignment </TextBlock>")    
		//_T("    <TextBlock Padding='15, 5, 5, 10' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MODEL_SELECTED 
		(
		_T("<Border Width='168' Height='118' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='3,3,3,3'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")    
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#003366'>복합수단 통행배정 모형</TextBlock>")    
		_T("    <Border Width='164' Height='1' Background='#7c96b5' />")
		_T("    <Border Width='164' Height='1' Background='#9acae4' />")
		_T("    <TextBlock TextAlignment='Center' Padding='0, 20, 0, 10' FontSize='11' Foreground='#000000'>수단선택, 대중교통 통행배정,</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontSize='11' Foreground='#000000'>도로 통행배정 통합 모형</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//mode define
	const CString
		UIXAML_IA_MODEDDEFINE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway Mode </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Parameter </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MODEDDEFINE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 관련 계수 설정 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_MODEDDEFINE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway Mode </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Parameter </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MODEDDEFINE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 관련 계수 설정 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_MODEDDEFINE_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway Mode </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Parameter </TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MODEDDEFINE_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>관련 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//VDF
	const CString
		UIXAML_IA_VDF_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway VDF </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VDF_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>통행지체함수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_VDF_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway VDF </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VDF_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>통행지체함수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_VDF_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#003366'> Highway VDF </TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VDF_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>통행지체함수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Highway Options
	const CString
		UIXAML_IA_HRUNOPTIONS_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_HRUNOPTIONS_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>실행 옵션 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_HRUNOPTIONS_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_HRUNOPTIONS_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>실행 옵션 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_HRUNOPTIONS_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_HRUNOPTIONS_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도로 통행배정</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>실행 옵션 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Mode_Route Choice Paramter
	const CString
		UIXAML_IA_MR_PARAMETER_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Urban </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MR_PARAMETER_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>도시부 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_MR_PARAMETER_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Urban </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MR_PARAMETER_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도시부 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_MR_PARAMETER_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Urban </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_MR_PARAMETER_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도시부 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Mode_Route Choice Paramter
	const CString
		UIXAML_IA_TM_PARAMETER_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Regional </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_TM_PARAMETER_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>지역간 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_TM_PARAMETER_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Regional </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_TM_PARAMETER_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>지역간 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_TM_PARAMETER_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Regional </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Choice Parameter </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_TM_PARAMETER_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>지역간 수단/경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//stoping criteria
	const CString
		UIXAML_IA_STOPPING_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Stopping Criteria </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_STOPPING_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_STOPPING_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Stopping Criteria </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_STOPPING_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_STOPPING_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#003366'> Stopping Criteria </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_STOPPING_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//output

	const CString
		UIXAML_IA_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_VIEWTABLE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		//_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW TABLE </TextBlock>")
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Result View </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VIEWTABLE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>결과보기</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_VIEWTABLE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		//_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> VIEW TABLE </TextBlock>")
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Result View </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VIEWTABLE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>결과보기</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);


	const CString
		UIXAML_IA_VIEWLOG_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW LOG </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VIEWLOG_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>로그 파일 보기</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_IA_VIEWLOG_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW LOG </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_IA_VIEWLOG_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>로그 파일 보기</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
}